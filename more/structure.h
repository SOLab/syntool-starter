#ifndef STRUCTURE_H
#define STRUCTURE_H
#include <QString>
#include <QCache>

namespace LogLevel {
    enum LogLevelValue{
        NoLogMessage = 0,
        DebugOnly = 1,
        WarningOnly = 2,
        DebugAndWarning = 3,
        ErrorOnly = 4,
        DebugAndError = 5,
        WarningAndError = 6,
        DebugWarningAndError = 7,
        Info = 8,
        AllMessage = 15
    };
}

namespace GranuleType {
    enum Type{
        Error = 0,
        Simple = 1,
        Tiled = 2
    };
}

namespace CursorMode {
    enum Mode{
        Move = 0,
        AddLine = 1,
        AddRect = 2,
        AddTag = 3,
        AddPin = 4,
        LeftTopCoords = 5,
        RightBottomCoords = 6
    };
}

struct ConfigData{
    QString configDir;
    QString configFile;

    QString                 serverName;
    QString                 lang;
    QString                 mapThemeName;
    LogLevel::LogLevelValue logLevel;

    QString cacheDir;
    qint32  numberCachedTiles;
    qint32  numberCachedSimpleGranules;
    qint32  numberCachedTiledGranules;

    qint32 timeLineMoveFrequency;
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

inline bool operator==(const TileNumber &tn1, const TileNumber &tn2)
{
    return (tn1.zoom == tn2.zoom && tn1.x == tn2.x && tn1.y == tn2.y);
}

struct TileRange{
    qint32 startX;
    qint32 startY;
    qint32 endX;
    qint32 endY;
    bool   end;
};
#endif // STRUCTURE_H
