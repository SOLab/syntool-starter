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
    qreal one_part = -M_PI/18.0;

    GeoCoords pos1;
    GeoCoords pos2;
    qreal cur_pos;

    pos1.lat = 85/180.0*M_PI;
    pos2.lat = -pos1.lat;

    for (int i = 0; i < 36; i++)
    {
        cur_pos = one_part * i;

        pos1.lon = cur_pos;
        pos2.lon = cur_pos;

        Line3DNode* lineNode = new Line3DNode(this);
        lineNode->setLineWidth(1.0f);
        lineNode->setLineColor(QColor(200, 200, 200, 200));

        lineNode->createLine(pos1, pos2);
        lines->append(lineNode);
    }

    // create parallels
    one_part = -85/(180.0*17)*M_PI;

    for (int i = -17; i < 18; i++)
    {
        cur_pos = one_part * i;

        pos1.lat = cur_pos;
        pos2.lat = cur_pos;

        Line3DNode* lineNode = new Line3DNode(this);
        lineNode->setLineWidth(1.0f);
        lineNode->setLineColor(QColor(200, 200, 200, 200));

        lineNode->createParallelLine(pos1, pos2);
        lines->append(lineNode);
    }
}

void Grid3D::draw(QGLPainter *painter)
{
    for (int i = 0; i < lines->size(); ++i) {
        lines->at(i)->draw(painter);
    }
}
