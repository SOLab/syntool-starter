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

    qint32 iteractionRectNumber;
    qint32 iteractionLineNumber;

signals:
    void addPointSignal(qint32 pointNumber, GeoCoords pos, QString pointName);
    void addLineSignal(qint32 lineNumber, GeoCoords pos1, GeoCoords pos2, QString lineName);
    void addRectSignal(qint32 rectNumber, GeoCoords pos1, GeoCoords pos2, QString rectName);
    void removeObjectSignal(Geometry::Type type, qint32 objNumber);
    
public slots:
    void addPoint(GeoCoords pos, Geometry::Type objectType);
    void addLine(GeoCoords pos1, GeoCoords pos2);
    qint32 addRect(GeoCoords pos1, GeoCoords pos2);
    void addIteractionRect(GeoCoords pos1, GeoCoords pos2);
    void moveIteractionRect(GeoCoords pos2);
    void removeIteractionRect();

    void removeObjectSlot(Geometry::Type type, qint32 objectNumber);
    
};

#endif // METAGLINFOCLASS_H
