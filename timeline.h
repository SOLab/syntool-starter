#ifndef TIMELINE_H
#define TIMELINE_H

#include <QGraphicsView>

class QGraphicsScene;
class QMouseEvent;

class TimeLine : public QGraphicsView
{
    Q_OBJECT

protected:
    void mouseMoveEvent(QMouseEvent *anEvent);
    void mousePressEvent(QMouseEvent *anEvent);
    /*void mouseReleaseEvent(QMouseEvent *anEvent);
    void wheelEvent(QWheelEvent *anEvent);*/

public:
    TimeLine(QWidget *aParent = 0);
    ~TimeLine();

private:
    QGraphicsScene *scene_;

    qreal prev_translate_;
    quint64 value_;
    quint64 prev_value_;

    void drawGraduation();
    void followTimeline();
};

#endif // TIMELINE_H
