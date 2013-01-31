#ifndef TIMELINE_H
#define TIMELINE_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include <QPoint>
#include <QMouseEvent>
#include <QDebug>
#include <QCursor>
#include <QTimer>

#include <QDateTime>
#include <QHBoxLayout>
#include <QPushButton>

#include <QList>
#include <qmath.h>
#include <additionalwidgets/calendar.h>
#include <more/ProductStructures.h>
#include <network/getgranules.h>

#pragma pack(push, 1)
struct geoPoint
{
    QDateTime timeSnapshot;
    // center of latitude and longitude
    float lat;
    float lon;
};

struct geoSegment
{
    QDateTime startTime;
    QDateTime endTime;
    // center of latitude and longitude
    float lat;
    float lon;
};
#pragma pack(pop)

class ProductsWidget;

struct param
{
    QDateTime maxDate;
    QDateTime minDate;
    QDateTime currentDate;
    QRect dayRect;
    QRect weekRect;
    bool moveDay;
    bool moveWeek;
    bool flagMovedIntervalos_;
    QPoint pos_;

    int markerDistance;
};

class TimeLine : public QWidget
{
    Q_OBJECT

public:
    TimeLine(QWidget *parent = 0);
    ~TimeLine();
    void paintEvent(QPaintEvent * pe);
    void setSelectedProducts(QHash<QString, selectedProduct>* _selectedProducts,
                             QHash<QString, Granule>* _granulesHash);
    void setObjectsPointer(ProductsWidget* _ProductsWgtPointer,
                           GetGranules* _getGranulesPointer);
    param control_;

protected:
    void createBottomRect();
    void createTopRect();
    Calendar* calendar;

    QList<geoPoint> geoPointList;
    QList<geoSegment> geoSegmentList;
    QHash<QString, selectedProduct>* selectedProducts;
    QHash<QString, Granule>* granulesHash;

    QImage imageGeoPoint;
    ProductsWidget* ProductsWgtPointer;
    GetGranules* getGranulesPointer;

    void drawAllMarkers();
    QDate notChangedDate;

public slots:
    void setDate();
    void setCurrentDate();

    void addGeoPoint(QDateTime dateTime, float lat, float lon);
    void addGeoSegment(QDateTime startDateTime, QDateTime endDateTime, float lat, float lon);

    void changedDay();

private slots:
    void mousePressEvent ( QMouseEvent * pe );
    void mouseMoveEvent(QMouseEvent * pe);
    void mouseReleaseEvent ( QMouseEvent * pe );

signals:
  void getNewAllGranules();

private:
};

#endif // TIMELINE_H
