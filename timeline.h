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
#include <additionalwidgets/calendar.h>

class TimeLine : public QWidget
{
    Q_OBJECT

public:
    TimeLine(QWidget *parent = 0);
    ~TimeLine();
    void paintEvent(QPaintEvent * pe);

protected:
    void createBottomRect();
    void createTopRect();
    Calendar* calendar;

public slots:
    void setDate();
    void setCurrentDate();

private slots:
    void mousePressEvent ( QMouseEvent * pe );
    void mouseMoveEvent(QMouseEvent * pe);
    void mouseReleaseEvent ( QMouseEvent * pe );

private:
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
    param control_;
};

#endif // TIMELINE_H
