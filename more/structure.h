#ifndef STRUCTURE_H
#define STRUCTURE_H
#include <QString>

struct ConfigData{
    QString serverName;
    QString cacheDir;
};

struct BoundingBox {
    double north;
    double south;
    double east;
    double west;
  };

struct GeoCoords{
    qreal lat;
    qreal lon;
};

struct TileNumber{
    qint32 x;
    qint32 y;
    qint32 zoom;
};

struct TileRange{
    qint32 startX;
    qint32 startY;
    qint32 endX;
    qint32 endY;
};
#endif // STRUCTURE_H
