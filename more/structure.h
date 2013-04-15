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

#endif // STRUCTURE_H
