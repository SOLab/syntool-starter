#include "point3dnode.h"

Point3DNode::Point3DNode(QObject *parent) :
    QObject(parent)
{
}

void Point3DNode::createPoint(Geometry::Type objectType, GeoCoords pos)
{
    pos.lat = -1*pos.lat;
    pos.lon += M_PI_2;
    pos.lon = -1*pos.lon;

    _pos = pos;
    qreal m_altitude = 0.01;
    QVector3D curDecart = llh2xyz(pos.lat, pos.lon, m_altitude);

    verts.clear();
    verts.append(curDecart.x(), curDecart.y(), curDecart.z());
//    verts.append(curDecart.x()+0.0001, curDecart.y()+0.0001, curDecart.z()+0.0001);

}

void Point3DNode::draw(QGLPainter *painter)
{
    glPointSize(4.0f);

    painter->clearAttributes();
    painter->setStandardEffect(QGL::FlatColor);

    painter->setVertexAttribute(QGL::Position, verts);
    painter->setColor(QColor(255,0,0));
    painter->draw(QGL::Points, 1);
}
