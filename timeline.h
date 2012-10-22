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

class TimeLine : public QWidget
{
    Q_OBJECT

public:
    TimeLine(QWidget *parent = 0);
    ~TimeLine();
    void paintEvent(QPaintEvent * pe);
    void checkMouse();

private slots:
    void mousePressEvent ( QMouseEvent * pe );
    void mouseMoveEvent(QMouseEvent * pe);
    void mouseReleaseEvent ( QMouseEvent * pe );
    void moveEnabled();
    void backToMouseAndCursor(bool inDirection);


private:
    struct param
    {
        int max_;
        int min_;
        int curMin_;
        int curMax_;
        int current_;
        QRect cursorRect_;
        bool move_;
        bool flagMovedIntervalos_;
        QPoint pos_;
    };
    param control_;
};

#endif // TIMELINE_H
