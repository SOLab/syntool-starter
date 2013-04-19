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

class TileCacheNumber
{
public:
    TileCacheNumber(const qint32 zoom,const qint32 x,const qint32 y):
      zoom(zoom),
      x(x),
      y(y)
    {}

//private:
    int zoom;
    int x;
    int y;
};

inline bool operator==(const TileCacheNumber &tcn1, const TileCacheNumber &tcn2)
{
    return (tcn1.zoom == tcn2.zoom && tcn1.x == tcn2.x && tcn1.y == tcn2.y);
}

inline uint qHash(const TileCacheNumber &key)
{
    return qHash(key.zoom) ^ qHash( key.x ) ^ qHash( key.y );
}

struct TileRange{
    qint32 startX;
    qint32 startY;
    qint32 endX;
    qint32 endY;
};
#endif // STRUCTURE_H
