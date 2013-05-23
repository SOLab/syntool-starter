#include "metagranules.h"

MetaGranules::MetaGranules(EarthView *parentView, QSharedPointer<QGLMaterialCollection> palette, ConfigData configData)
{
    m_palette = palette;
    m_configData = configData;
    setParent(parentView);

    simpleGranuleCache = new QCache<int, SimpleGranulesNode>;
    simpleGranuleCache->setMaxCost(50);
}

void MetaGranules::draw(QGLPainter *painter)
{
    QList<qint32>::iterator i;
    QList<qint32> keys = simpleGranuleCache->keys();

    for (i = keys.begin(); i != keys.end(); ++i)
    {
        qint32 j = *i;
        qCritical() << "!!!!!!!!!!!!!" << j;
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

void MetaGranules::addGranuleNode(qint32 granuleId, qint32 productId)
{
    addSimpleGranuleNode(granuleId, productId);
}


void MetaGranules::addSimpleGranuleNode(qint32 granuleId, qint32 productId)
{
    if (!simpleGranuleCache->contains(granuleId))
    {
        SimpleGranulesNode* granulesNode = new SimpleGranulesNode(this, m_palette, m_configData, granuleId, productId);
        simpleGranuleCache->insert(granuleId, granulesNode);
    }

//    connect (this, &EarthView::updatedTilesSignal, earth, &Earth::updateTilesSlot);
//        connect (granulesNode, &SimpleGranulesNode::displayed, parent, &EarthView::update);

    //    m_scene->mainNode()->addNode(granulesNode);
}

void MetaGranules::removeSimpleGranuleNode(qint32 granuleId, qint32 productId)
{
//    qCritical() << "REMOVE!!!!!" << granuleId;
    simpleGranuleCache->remove(granuleId);
}
