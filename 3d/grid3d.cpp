#include "grid3d.h"

Grid3D::Grid3D(QObject *parent) :
    QObject(parent)
{
    lines = new QList<Line3DNode*>;
}

Grid3D::~Grid3D()
{
    qDeleteAll(*lines);
    lines->clear();
    delete lines;
}

void Grid3D::createGrid()
{
    // create meridians
    qreal start = -M_PI;
    qreal df = M_PI/18.0;

    GeoCoords pos1;
    GeoCoords pos2;

    pos1.lat = 85/180.0*M_PI;
    pos2.lat = -pos1.lat;

    for (int i = 0; i < 36; i++)
    {
        start += df;

        pos1.lon = start;
        pos2.lon = start;

        Line3DNode* lineNode = new Line3DNode(this);
        lineNode->setLineWidth(1.0f);
        lineNode->setLineColor(QColor(200, 200, 200, 200));

        lineNode->createLine(pos1, pos2);
        lines->append(lineNode);
    }

    // create parallels
    start = -85/180.0*M_PI;
    df = pos1.lat/17;

    for (int i = 0; i < 35; i++)
    {
        pos1.lat = start;
        pos2.lat = start;

        Line3DNode* lineNode = new Line3DNode(this);
        lineNode->setLineWidth(1.0f);
        lineNode->setLineColor(QColor(200, 200, 200, 200));

        lineNode->createParallelLine(pos1, pos2);
        lines->append(lineNode);

        start += df;
    }
}

void Grid3D::draw(QGLPainter *painter)
{
    for (int i = 0; i < lines->size(); ++i) {
        lines->at(i)->draw(painter);
    }
}
