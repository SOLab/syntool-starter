#include "metagranules.h"

MetaGranules::MetaGranules(EarthView *parentView, QSharedPointer<QGLMaterialCollection> palette, ConfigData configData)
{
    m_palette = palette;
    m_configData = configData;
    setParent(parentView);
    currentHeight = 1;

    simpleGranuleCache = new QCache<int, SimpleGranulesNode>;
    simpleGranuleCache->setMaxCost(50);
}

void MetaGranules::drawSimpleGranules(QGLPainter *painter)
{
    qCritical() << heightGranuleMap.keys();

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

void MetaGranules::addGranuleNode(qint32 granuleId, qint32 productId)
{
    addSimpleGranuleNode(granuleId, productId);
}

void MetaGranules::addSimpleGranuleNode(qint32 granuleId, qint32 productId)
{
    if (!simpleGranuleCache->contains(granuleId))
    {
        SimpleGranulesNode* granulesNode = new SimpleGranulesNode(this, m_palette, m_configData, granuleId, productId);
        granulesNode->setHeight(currentHeight);
        simpleGranuleCache->insert(granuleId, granulesNode);
        heightGranuleMap.insert(currentHeight, granuleId);
        currentHeight++;
        connect(granulesNode, &SimpleGranulesNode::updated, this, &MetaGranules::displayed);
    }
    else
    {
        // if granule already displayed
        if (simpleGranuleCache->object(granuleId)->options() == QGLSceneNode::CullBoundingBox)
        {
            return;
        }

        // displayed granule
        simpleGranuleCache->object(granuleId)->setOptions(QGLSceneNode::CullBoundingBox);
//        simpleGranuleCache->object(granuleId)->setHeight(currentHeight);
        heightGranuleMap.insert(simpleGranuleCache->object(granuleId)->height(), granuleId);
        heightGranuleMap.insert(simpleGranuleCache->object(granuleId)->height(), granuleId);
        emit displayed();
    }

//    connect (this, &EarthView::updatedTilesSignal, earth, &Earth::updateTilesSlot);
//        connect (granulesNode, &SimpleGranulesNode::displayed, parent, &EarthView::update);

    //    m_scene->mainNode()->addNode(granulesNode);
}

void MetaGranules::removeSimpleGranuleNode(qint32 granuleId, qint32 productId)
{
    Q_UNUSED(productId);

    // hide granule
    simpleGranuleCache->object(granuleId)->options().testFlag(QGLSceneNode::HideNode);
    heightGranuleMap.remove(simpleGranuleCache->object(granuleId)->height());
//    simpleGranuleCache->object(granuleId)->deleteLater();
//    simpleGranuleCache->remove(granuleId);

    emit displayed();
}
