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

#include <QMessageBox>
#include <QDateTime>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>

#include <QList>
#include <qmath.h>
#include "additionalwidgets/calendar.h"
#include "additionalwidgets/timelineplayer.h"
#include "additionalwidgets/granuleinfowidget.h"
#include "more/ProductStructures.h"
#include "more/granuleactions.h"
#include "more/structure.h"
#include "network/getgranules.h"
#include "network/getgranulecoords.h"

#include <QDesktopServices>


#pragma pack(push, 1)
struct geoPoint
{
    QDateTime timeSnapshot;
    // center of latitude and longitude
    float lat;
    float lon;
};

inline bool operator==(const geoPoint &gp1, const geoPoint &gp2)
{
    return (gp1.lat == gp2.lat && gp1.lon == gp2.lon && gp1.timeSnapshot == gp2.timeSnapshot);
}

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
    QRect     dayRect;
    QRect     weekRect;
    bool      moveDay;
    bool      moveWeek;
    bool      flagMovedIntervalos_;
    QPoint    pos_;

    int markerDistance;
};

class TimeLine : public QWidget
{
    Q_OBJECT

public:
    TimeLine(ConfigData *configData, QWidget *parent = 0);
    ~TimeLine();
    void paintEvent(QPaintEvent * pe);
    void setSelectedProducts(QHash<QString, selectedProduct>* selectedProductsValue,
                             QHash<QString, Granule>* granulesHashValue);
    param control_;
    void setFixedDate(QDateTime dateTime);

protected:
    void createBottomRect();
    void createTopRect();
    void createGranulesContextMenu();
    void drawAllMarkers();

    Calendar* calendar;
    TimeLinePlayer* timelinePlayer;

    QList<geoPoint>*                 geoPointList;
    QList<geoSegment>                geoSegmentList;
    QHash<QString, selectedProduct>* selectedProducts;
    QHash<QString, Granule>*         granulesHash;
    QHash<qint32, QRect>*            rectsGranules;
    // <granuleId, productId>
    QHash<qint32, qint32>*           displayedGranules;
    QList<QString>                   GranuleStatuses;

    QImage       imageGeoPoint;
    GetGranules* getGranulesPointer;
    QDate        notChangedDate;
    QMenu*       granulesContextMenu;
    qint32       currentGranuleId;
    QString      serverName;


    int   hourPixels;
    float minutePixels;
    float secondsInPixel;

public slots:
    void setDate();
    void setCurrentDate();
    void plusTime(qint32 minutes);
    void minusTime(qint32 minutes);

    void addGeoPoint(QDateTime dateTime, qint32 granuleId, float lat, float lon, int productId);
    void addGeoSegment(QDateTime startDateTime, QDateTime endDateTime, float lat, float lon);

    void changedDay();
    void setCoordsGranule(qint32 granuleId, float north, float east, float south, float west);

private slots:
    void mousePressEvent (QMouseEvent * pe);
    void mouseMoveEvent(QMouseEvent * pe);
    void mouseReleaseEvent (QMouseEvent * pe);
    void wheelEvent(QWheelEvent *pe);

    void granulePressLeft();
    void granulePressRight();

    void actionImageSlot();
    void actionOpendapSlot();
    void actionFtpSlot();
    void actionKmlSlot();
    void actionPropertiesSlot();

signals:
    void getNewAllGranules(int scale);
    void changedDisplayGranules(QHash<qint32, qint32>* displayedGranules);
    void moveEarth(float lat, float lot);

private:
};

#endif // TIMELINE_H
