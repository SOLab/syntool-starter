#include "line3dnode.h"

Line3DNode::Line3DNode(QObject *parent) :
    QObject(parent)
{
    _lineWidth = 2.0f;
    _color = QColor(255,0,0);
}

void Line3DNode::createLine(GeoCoords pos1, GeoCoords pos2)
{
    verts.clear();

    pos1.lat = -1*pos1.lat;
    pos1.lon += M_PI_2;
    pos1.lon = -1*pos1.lon;

    pos2.lat = -1*pos2.lat;
    pos2.lon += M_PI_2;
    pos2.lon = -1*pos2.lon;

    // all stacks and slices
    qint32 stacks = 128;
    qint32 slices = 128;
    qint32 separation = 1;

    if (qAbs(pos2.lon - pos1.lon) > M_PI)
    {
        if (pos2.lon > 0)
            pos2.lon -= 2*M_PI;
        else
            pos2.lon += 2*M_PI;
    }

    qreal oneStackDegrees = (pos2.lat - pos1.lat) / (stacks/(float)separation);
    qreal oneSliceDegrees = (pos2.lon - pos1.lon) / (slices/(float)separation);

    QVector3D curDecart;
    QVector3D curDecartNext;

    // maxSphereLon with with an error of calculations (approximately)
//    qreal maxSphereLonApprox = pos2.lon+0.0001;
    qreal m_altitude = 0.00001;

    qreal curLon = pos1.lon;
    qreal curLat = pos1.lat;
    qreal curLatNext;
    qreal curLonNext;

    for (int i = 0; i < slices; i++)
    {
        // if next latitude is max latitude
        if (qAbs(curLat - pos2.lat) < 0.001)
        {
            curLat = pos2.lat;
        }
        curLatNext = curLat + oneStackDegrees;

        // if next lon is max lon
        if (qAbs(curLon - pos2.lon) < 0.001)
        {
            curLon = pos2.lon;
        }
        curLonNext = curLon + oneSliceDegrees;

//        prim.clear();

        // calculate decart coordinates (x,y,z) for Vertex and Normal
        curDecart = llh2xyz(curLat, curLon, m_altitude);
        curDecartNext = llh2xyz(curLatNext, curLonNext, m_altitude);
        verts.append(curDecart.x(), curDecart.y(), curDecart.z());
        verts.append(curDecartNext.x(), curDecartNext.y(), curDecartNext.z());

        curLon += oneSliceDegrees;
        curLat += oneStackDegrees;
    }
}

void Line3DNode::createParallelLine(GeoCoords pos1, GeoCoords pos2)
{
    verts.clear();

    pos1.lat = -1*pos1.lat;
    pos1.lon = -M_PI;

    pos2.lat = -1*pos2.lat;
    pos2.lon = M_PI;

    // all stacks and slices
    qint32 stacks = 128;
    qint32 slices = 128;

    qreal oneStackDegrees = (pos2.lat - pos1.lat) / qreal(stacks);
    qreal oneSliceDegrees = (pos2.lon - pos1.lon) / qreal(slices);

    QVector3D curDecart;
    QVector3D curDecartNext;

    // maxSphereLon with with an error of calculations (approximately)
//    qreal maxSphereLonApprox = pos2.lon+0.0001;
    qreal m_altitude = 0.001;// + height()/10000.0;

    qreal curLon = pos1.lon;
    qreal curLat = pos1.lat;
    qreal curLatNext;
    qreal curLonNext;

    for (int i = 0; i < slices; i++)
    {
        // if next latitude is max latitude
        if (qAbs(curLat - pos2.lat) < 0.001)
        {
            curLat = pos2.lat;
        }
        curLatNext = curLat + oneStackDegrees;

        // if next lon is max lon
        if (qAbs(curLon - pos2.lon) < 0.001)
        {
            curLon = pos2.lon;
        }
        curLonNext = curLon + oneSliceDegrees;

//        prim.clear();

        // calculate decart coordinates (x,y,z) for Vertex and Normal
        curDecart = llh2xyz(curLat, curLon, m_altitude);
        curDecartNext = llh2xyz(curLatNext, curLonNext, m_altitude);
        verts.append(curDecart.x(), curDecart.y(), curDecart.z());
        verts.append(curDecartNext.x(), curDecartNext.y(), curDecartNext.z());

        curLon += oneSliceDegrees;
        curLat += oneStackDegrees;
    }
}

void Line3DNode::draw(QGLPainter *painter)
{
    glLineWidth(_lineWidth);

    painter->clearAttributes();
    painter->setStandardEffect(QGL::FlatColor);

    painter->setVertexAttribute(QGL::Position , verts);
    painter->setColor(_color);
    painter->draw(QGL::Lines,verts.size());
}
