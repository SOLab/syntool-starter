#ifndef GEOFUNCTIONS_H
#define GEOFUNCTIONS_H

#include "qmath.h"
#include "more/structure.h"
#include <QVector3D>

const qint32 lEarth = 40075.017;

inline double mercator(double x) {
    return 0.5*log((1.0+qSin(x))/(1.0-qSin(x)));
}

const double maxAbsLon = 180.0;
const double maxAbsLat = 85.051128;
#define maxAbsLatMer 85.051128
const double defMercAngle = maxAbsLat * M_PI / 180.0;
const double defMercScale = M_PI_2 / mercator(defMercAngle);

const double sphereRarius = 0.5;

inline double Mercator2SphereAnalytic(double iTY, const double scale = defMercScale,
                               const double maxAng = defMercAngle)
{
    double angle = (iTY * 2 - 1) * M_PI_2;		// texture V to angle
    double angle2 = fabs(angle);
    double val = (angle2 > maxAng) ? M_PI_2 : (mercator(angle2) * scale);
    if (angle < 0.0) val = -val;
    return (1 + val / M_PI_2) * 0.5;	// angle to texture V
}


#define RADIANS_PER_DEGREE M_PI/180.0
inline double Lat2MercatorLatitude(double latitude)
{
    latitude = latitude*180.0/M_PI;
    double sign = (latitude < 0) ? -1 : 1;
    double sin = qSin(latitude * RADIANS_PER_DEGREE * sign);
    return sign * (qLn((1.0 + sin) / (1.0 - sin)) / 2.0);
}

inline QVector3D llh2xyz(qreal _lat, qreal _lon, qreal h = 0)
{
    qreal lat = _lat;//*M_PI/180.0;
    qreal lon = _lon;//*M_PI/180.0;
    qreal e2 = 0.00669436619;
    qreal N = sphereRarius/qSqrt(1 - e2 * qPow(qSin(lat),2));

    QVector3D decartCoordPoint;

    decartCoordPoint.setX( (N + h)*qCos(lat)*qCos(lon) );
    decartCoordPoint.setY( (N + h)*qCos(lat)*qSin(lon) );
    decartCoordPoint.setZ( (N*(1-e2)+h)*qSin(lat) );

    return decartCoordPoint;
}

inline GeoCoords ecef2wgs84Rad(double x, double y, double z)
{
    GeoCoords pos;

    pos.lat=atan2(z,qSqrt(x*x+y*y));
    pos.lon=atan2(y,x);
    return pos;
}

inline GeoCoords ecef2wgs84Deg(double x, double y, double z)
{
    GeoCoords pos;

    pos.lat=atan2(z,qSqrt(x*x+y*y))/M_PI*180;
    pos.lon=atan2(y,x)/M_PI*180;
    return pos;
}
inline qreal tiley2lat(int y, int separation)
{
    double n = M_PI - 2.0 * M_PI * y / qreal(separation);
    return 180.0 / M_PI * qAtan(0.5 * (qExp(n) - qExp(-n)));
}

inline qreal deg2rad(qreal degrees)
{
    return degrees*M_PI/180.0;
}

inline TileNumber deg2TileNum(GeoCoords geoCoords, int zoom)
{
    qreal lat_rad = deg2rad(geoCoords.lat);
    int n = qPow(2, zoom);
    TileNumber tileNumber;
    tileNumber.x = int((geoCoords.lon + 180.0) / 360.0 * n);
    tileNumber.y = int((1.0 - qLn(qTan(lat_rad) + (1 / qCos(lat_rad))) / M_PI) / 2.0 * n);
    return tileNumber;
}

/*
 * the number of tiles included in the region (visible distance)
 * lEarth - circumference of the earth (in km)
 * Rt - the number of km in each tile
 * Ntiles - the number of tiles
 */
inline qint32 getNumberTiles(int zoom, qreal visibleDistance)
{
    qreal Rt = lEarth/qPow(2, zoom);
    qreal Ntiles = visibleDistance/Rt;
    return qCeil(Ntiles);
}

/*
 * range of tile numbers (maximum 3)
 */
inline TileRange *getTileRange(TileNumber NCenterTile, qint32 rangeTiles, qint32 zoom)
{
    // get maximum
    TileRange *tileRanges=new TileRange[2];
//    TileRange tileRanges[2];
    qint32 maxTileNumber = qPow(2,zoom) - 1;

    /*
     * ..xxx
     * ..xxx
     * ..xxx
     */
    // right
    if(NCenterTile.x + rangeTiles > maxTileNumber)
    {
        // right-bottom
        if(NCenterTile.y + rangeTiles > maxTileNumber)
        {
            tileRanges[0].startX = 0;
            tileRanges[0].endX = maxTileNumber;

            tileRanges[0].startY = NCenterTile.y - rangeTiles;
            tileRanges[0].endY = maxTileNumber;

            tileRanges[1].startX = 0;
            tileRanges[1].startY = 0;
            tileRanges[1].endX = 0;
            tileRanges[1].endY = 0;
        }

        // right-top
        else if(NCenterTile.y - rangeTiles < 0)
        {
            tileRanges[0].startX = 0;
            tileRanges[0].endX = maxTileNumber;

            tileRanges[0].startY = 0;
            tileRanges[0].endY = NCenterTile.y + rangeTiles;

            tileRanges[1].startX = 0;
            tileRanges[1].startY = 0;
            tileRanges[1].endX = 0;
            tileRanges[1].endY = 0;
        }

        // right-center
        else
        {
            tileRanges[0].startX = 0;
            tileRanges[0].endX = NCenterTile.x + rangeTiles - maxTileNumber;
            tileRanges[0].startY = NCenterTile.y - rangeTiles;
            tileRanges[0].endY = NCenterTile.y + rangeTiles;

            tileRanges[1].startX = NCenterTile.x - rangeTiles;
            tileRanges[1].endX = maxTileNumber;
            tileRanges[1].startY = NCenterTile.y - rangeTiles;
            tileRanges[1].endY = NCenterTile.y + rangeTiles;
        }
    }

    /*
     * xxx..
     * xxx..
     * xxx..
     */
    // left
    else if(NCenterTile.x - rangeTiles < 0)
    {
        // left-bottom
        if(NCenterTile.y + rangeTiles > maxTileNumber)
        {
            tileRanges[0].startX = 0;
            tileRanges[0].endX = maxTileNumber;

            tileRanges[0].startY = NCenterTile.y - rangeTiles;
            tileRanges[0].endY = maxTileNumber;

            tileRanges[1].startX = 0;
            tileRanges[1].startY = 0;
            tileRanges[1].endX = 0;
            tileRanges[1].endY = 0;
        }

        // left-top
        else if(NCenterTile.y - rangeTiles < 0)
        {
            tileRanges[0].startX = 0;
            tileRanges[0].endX = maxTileNumber;

            tileRanges[0].startY = 0;
            tileRanges[0].endY = NCenterTile.y + rangeTiles;

            tileRanges[1].startX = 0;
            tileRanges[1].startY = 0;
            tileRanges[1].endX = 0;
            tileRanges[1].endY = 0;
        }

        // left-center
        else
        {
            tileRanges[0].startX = 0;
            tileRanges[0].endX = NCenterTile.x + rangeTiles;
            tileRanges[0].startY = NCenterTile.y - rangeTiles;
            tileRanges[0].endY = NCenterTile.y + rangeTiles;

            tileRanges[1].startX = maxTileNumber + NCenterTile.x - rangeTiles;
            tileRanges[1].endX = maxTileNumber;
            tileRanges[1].startY = NCenterTile.y - rangeTiles;
            tileRanges[1].endY = NCenterTile.y + rangeTiles;
        }
    }

    /*
     * .xxx.
     * .xxx.
     * .xxx.
     */
    else
    {
        tileRanges[0].startX = NCenterTile.x - rangeTiles;
        tileRanges[0].endX = NCenterTile.x + rangeTiles;

        // center-bottom
        if(NCenterTile.y + rangeTiles > maxTileNumber)
        {
            tileRanges[0].startY = NCenterTile.y - rangeTiles;
            tileRanges[0].endY = maxTileNumber;

            tileRanges[1].startX = 0;
            tileRanges[1].endX = maxTileNumber;
            tileRanges[1].startY = 2*maxTileNumber - NCenterTile.y - rangeTiles;
            tileRanges[1].endY = maxTileNumber;
        }

        // center-top
        else if(NCenterTile.y - rangeTiles < 0)
        {
            tileRanges[0].startY = 0;
            tileRanges[0].endY = NCenterTile.y + rangeTiles;

            tileRanges[1].startX = 0;
            tileRanges[1].endX = maxTileNumber;
            tileRanges[1].startY = 0;
            tileRanges[1].endY = rangeTiles - NCenterTile.y - 1;
        }

        // center-center
        else
        {
            tileRanges[0].startY = NCenterTile.y - rangeTiles;
            tileRanges[0].endY = NCenterTile.y + rangeTiles;

            tileRanges[1].startX = 0;
            tileRanges[1].startY = 0;
            tileRanges[1].endX = 0;
            tileRanges[1].endY = 0;
        }
    }

    return tileRanges;
}

#endif // GEOFUNCTIONS_H
