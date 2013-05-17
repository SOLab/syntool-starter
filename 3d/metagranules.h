#ifndef METAGRANULES_H
#define METAGRANULES_H

#include <QObject>
#include <QGLSceneNode>
#include "more/structure.h"
#include "tilecacheclass.h"

class MetaGranules : public QObject
{
    Q_OBJECT
public:
    explicit MetaGranules(QObject *parent = 0);
    
private:
    // int idGranule
    QCache<int, QGLSceneNode> simpleGranuleCache;
    QCache<TileCacheNumber, QGLSceneNode> tiledGranuleCache;

signals:
    
public slots:
    
};

#endif // METAGRANULES_H
