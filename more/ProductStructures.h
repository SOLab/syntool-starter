#ifndef PRODUCTSTRUCTURES_H
#define PRODUCTSTRUCTURES_H

#include <QDateTime>
#include <QStringList>

struct Granule{
    qint32 granuleId;
//    QString productName;
    int productId;
    int status;
    QString granuleName;
    QDateTime startDate;
//    QDateTime endDate;
    float centerLat;
    float centerLon;
    float north;
    float east;
    float south;
    float west;
};

struct Product{
    QString Description;
    QString ImageUrl;
    QString NaiadProductId;
    QString Name;
    QString ProcessingCenters;
    QString ProcessingLevels;
    QString ProductionInterval;
    QString SpatialResolutions;

    QStringList Platforms;
    QStringList Sensors;

    bool IsTiled;
    bool IsGlobalCoverage;
    bool IsMapModeSupported;
    bool IsPresentationModeSupported;
    bool IsPrivate;
    bool NoQuiklooks;

    QHash<QString, int> productsParameters;

    QDateTime StartDate;
    QDateTime EndDate;
    int FilteredGranulesCount;
    qint32 Id;
    int TotalGranulesCount;
};

struct selectedProduct{
    QString productName;
    float   west;
    float   east;
    float   south;
    float   north;
    QString parameter;
    qint32  id;
};

#endif // PRODUCTSTRUCTURES_H
