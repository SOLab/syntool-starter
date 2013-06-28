#ifndef LINE3DNODE_H
#define LINE3DNODE_H

#include <QObject>
#include <QColor>
#include <QVector3D>
#include <QGLPainter>

#include "more/structure.h"
#include "more/geofunctions.h"

class Line3DNode : public QObject
{
    Q_OBJECT
public:
    explicit Line3DNode(QObject *parent = 0);
    void createLine(GeoCoords pos1, GeoCoords pos2);
    void draw(QGLPainter *painter);
    
private:
    QColor _color;
    GeoCoords _pos1;
    GeoCoords _pos2;
    QVector3DArray verts;

signals:
    
public slots:
    
};

#endif // LINE3DNODE_H
