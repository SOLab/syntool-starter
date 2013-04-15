#ifndef GEOFUNCTIONS_H
#define GEOFUNCTIONS_H

#include "qmath.h"
#include <QVector3D>

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


struct geoDetic
{
  double alt, lat, lon;
};

inline geoDetic ecef2wgs84(double x, double y, double z)
{
    geoDetic pos;
    pos.alt = 0;

    pos.lat=atan2(z,qSqrt(x*x+y*y));
    pos.lon=atan2(y,x);
    return pos;
}

#endif // GEOFUNCTIONS_H
