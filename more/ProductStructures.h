#ifndef PRODUCTSTRUCTURES_H
#define PRODUCTSTRUCTURES_H

struct Granule{
    qint32 granuleId;
    QString productName;
    int productId;
    QString granuleName;
    QDateTime startDate;
    QDateTime endDate;
    float lat;
    float lon;
};

struct Product{
    QString Description;
    QString ImageUrl;
    QString NaiadProductId;
    QString Name;
    QStringList Platforms;
    QString ProcessingCenters;
    QString ProcessingLevels;
    QString ProductionInterval;
    QStringList Sensors;
    QString SpatialResolutions;

    bool IsGlobalCoverage;
    bool IsMapModeSupported;
    bool IsOnGoing;
    bool IsPresentationModeSupported;
    bool IsPrivate;
    bool NoQuiklooks;

    QHash<QString, int> productsParameters;

    QDateTime StartDate;
    QDateTime EndDate;
    int FilteredGranulesCount;
    int Id;
    int TotalGranulesCount;
};

struct selectedProduct{
    QString productName;
    float west;
    float east;
    float south;
    float north;
    QString parameter;
};

#endif // PRODUCTSTRUCTURES_H
