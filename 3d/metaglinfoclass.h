#ifndef METAGLINFOCLASS_H
#define METAGLINFOCLASS_H

#include <QGLSceneNode>
#include <QMap>

#include "more/structure.h"
#include "earth.h"
#include "line3dnode.h"
#include "point3dnode.h"
#include "rect3dnode.h"

class EarthView;

class MetaGLInfoClass : public QGLSceneNode
{
    Q_OBJECT
public:
    explicit MetaGLInfoClass(EarthView *parentView, Earth *earth, QSharedPointer<QGLMaterialCollection> palette, ConfigData *configData);
    void drawAll(QGLPainter *painter);
    QGLSceneNode* m_floor;

private:
    QSharedPointer<QGLMaterialCollection> m_palette;
    ConfigData* m_configData;

    QVector3DArray verts;

    QMap<qint32, Line3DNode*>* lineHash;
    QMap<qint32, Point3DNode*>* pointHash;
    QMap<qint32, Rect3DNode*>* rectHash;

signals:
    
public slots:
    void addPoint(GeoCoords pos);
    void addLine(GeoCoords pos1, GeoCoords pos2);
    void addRect(GeoCoords pos1, GeoCoords pos2);
    
};

#endif // METAGLINFOCLASS_H
