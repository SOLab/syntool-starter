#include "metagranules.h"

MetaGranules::MetaGranules(EarthView *parentView, QSharedPointer<QGLMaterialCollection> palette, ConfigData *configData)
{
    m_palette = palette;
    m_configData = configData;
    setParent(parentView);

    simpleGranuleCache = new QCache<int, SimpleGranulesNode>;
    simpleGranuleCache->setMaxCost(configData->numberCachedSimpleGranules);
}

void MetaGranules::drawSimpleGranules(QGLPainter *painter)
{
    if (heightGranuleMap.keys().isEmpty())
        return;

    QList<qint32>::iterator i;
    QList<qint32> keys = heightGranuleMap.keys();

    for (i = keys.begin(); i != keys.end(); ++i)
    {
        qint32 j = heightGranuleMap.value(*i);
        if (simpleGranuleCache->contains(j))
        {
            SimpleGranulesNode* nodeToDraw = simpleGranuleCache->object(j);
            if (nodeToDraw)
            {
                nodeToDraw->draw(painter);
            }
        }
    }
}

void MetaGranules::drawTiledGranules(QGLPainter *painter)
{
    Q_UNUSED(painter);
}

void MetaGranules::setParent(EarthView *parentView)
{
    m_parentView = (QGLView*)parentView;
}

void MetaGranules::addGranuleNode(qint32 granuleId, qint32 productId, qint32 height, GranuleType::Type type)
{
    addSimpleGranuleNode(granuleId, productId, height);
}

void MetaGranules::addSimpleGranuleNode(qint32 granuleId, qint32 productId, qint32 height)
{
    if (!simpleGranuleCache->contains(granuleId))
    {
        SimpleGranulesNode* granulesNode = new SimpleGranulesNode(this, m_palette, m_configData, granuleId, productId,
                                           m_productsHash->value(m_productsIdName->value(productId)).IsGlobalCoverage);
        granulesNode->setHeight(height);
        granulesNode->show();
        simpleGranuleCache->insert(granuleId, granulesNode);
        heightGranuleMap.insert(height, granuleId);
//        registerPicking(granulesNode);
        connect(granulesNode, &SimpleGranulesNode::updated, this, &MetaGranules::displayed);
    }
    else
    {
        // displayed granule
        if (!heightGranuleMap.contains(simpleGranuleCache->object(granuleId)->height()))
            heightGranuleMap.insert(simpleGranuleCache->object(granuleId)->height(), granuleId);
        simpleGranuleCache->object(granuleId)->show();
        emit displayed();
    }
}

void MetaGranules::removeSimpleGranuleNode(qint32 granuleId, qint32 productId)
{
    Q_UNUSED(productId);
    if(simpleGranuleCache->object(granuleId))
    {
        heightGranuleMap.remove(simpleGranuleCache->object(granuleId)->height());
        simpleGranuleCache->object(granuleId)->hide();
    }

    emit displayed();
}

void MetaGranules::setGranuleTransparency(qint32 granuleId, qint32 productId, qint32 transparency)
{
    Q_UNUSED(productId);
    if (simpleGranuleCache->contains(granuleId))
        simpleGranuleCache->object(granuleId)->setTransparency(granuleId, transparency);
}

void MetaGranules::setProductsHashSlot(QHash<QString, Product> *productsHash, QHash<qint32, QString> *productsIdName)
{
    m_productsHash = productsHash;
    m_productsIdName = productsIdName;
}

void MetaGranules::changedGranuleHeight(qint32 granuleId, qint32 height)
{
    if (simpleGranuleCache->object(granuleId))
    {
        qint32 oldHeight = simpleGranuleCache->object(granuleId)->height();
        heightGranuleMap.remove(oldHeight);
        heightGranuleMap.insert(height, granuleId);
        simpleGranuleCache->object(granuleId)->setHeight(height);
        simpleGranuleCache->object(granuleId)->rebuildGranuleNode();
    }
}

void MetaGranules::changedGranulesHeight(qint32 granuleId1, qint32 height1, qint32 granuleId2, qint32 height2)
{
    if (simpleGranuleCache->object(granuleId1) && simpleGranuleCache->object(granuleId1))
    {
        qint32 oldHeight1 = height2;
        qint32 oldHeight2 = height1;

        heightGranuleMap.remove(oldHeight1);
        heightGranuleMap.remove(oldHeight2);

        heightGranuleMap.insert(height1, granuleId1);
        simpleGranuleCache->object(granuleId1)->setHeight(height1);

        heightGranuleMap.insert(height2, granuleId2);
        simpleGranuleCache->object(granuleId2)->setHeight(height2);

        simpleGranuleCache->object(granuleId1)->rebuildGranuleNode();
        simpleGranuleCache->object(granuleId2)->rebuildGranuleNode();

        emit displayed();
    }
}

void MetaGranules::registerPicking(SimpleGranulesNode* granulesNode)
{
    granulesNode->setPickNode(new QGLPickNode);
    QGLPickNode *node = granulesNode->pickNode();

    m_parentView->registerObject(node->id(), node);
    connect(node, SIGNAL(clicked()), this, SLOT(objectPicked()));
}

void MetaGranules::unregisterPicking()
{
}

void MetaGranules::objectPicked()
{
    Q_ASSERT(m_treeView);
    QGLPickNode *node = qobject_cast<QGLPickNode*>(sender());
    Q_ASSERT(node);
    SimpleGranulesNode *target = qobject_cast<SimpleGranulesNode*>(node->target());
}
