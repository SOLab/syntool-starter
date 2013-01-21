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

private slots:
    void mousePressEvent ( QMouseEvent * pe );
    void mouseMoveEvent(QMouseEvent * pe);
    void mouseReleaseEvent ( QMouseEvent * pe );
    void moveEnabled();


private:
    struct param
    {
        QDateTime maxDate;
        QDateTime minDate;
//        QDateTime curMinDate;
//        QDateTime curMaxDate;
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
