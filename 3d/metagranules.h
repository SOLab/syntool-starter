#ifndef METAGRANULES_H
#define METAGRANULES_H

#include <QObject>
#include <QGLSceneNode>
#include "more/structure.h"
#include "tilecacheclass.h"
#include "simplegranulesnode.h"

class EarthView;

class MetaGranules : public QGLSceneNode
{
    Q_OBJECT
public:
    MetaGranules(EarthView *parentView, QSharedPointer<QGLMaterialCollection> palette, ConfigData configData);
    void draw(QGLPainter *painter);
    void setParent(EarthView *parentView){_parentView = parentView;}

private:
    // int idGranule
    QCache<int, QGLSceneNode> simpleGranuleCache;
    QCache<TileCacheNumber, QGLSceneNode> tiledGranuleCache;
    SimpleGranulesNode *granulesNode;

    EarthView *_parentView;
    QSharedPointer<QGLMaterialCollection> m_palette;
    ConfigData m_configData;

signals:
    void displayed();
    
public slots:
    void addSimpleGranuleNode();
    
};

#endif // METAGRANULES_H
