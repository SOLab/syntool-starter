#include "metagranules.h"

MetaGranules::MetaGranules(EarthView *parentView, QSharedPointer<QGLMaterialCollection> palette, ConfigData configData)
{
    m_palette = palette;
    m_configData = configData;
    setParent(parentView);
    addSimpleGranuleNode();
}

void MetaGranules::draw(QGLPainter *painter)
{
    granulesNode->draw(painter);
}

void MetaGranules::addSimpleGranuleNode()
{

    granulesNode = new SimpleGranulesNode(this, m_palette, m_configData);
//    connect (this, &EarthView::updatedTilesSignal, earth, &Earth::updateTilesSlot);
//        connect (granulesNode, &SimpleGranulesNode::displayed, parent, &EarthView::update);

//    m_scene->mainNode()->addNode(granulesNode);
}
