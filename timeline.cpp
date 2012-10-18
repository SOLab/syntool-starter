#include "timeline.h"

#include <QGLWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMouseEvent>

using namespace Qt;

TimeLine::TimeLine(QWidget *aParent)
    : QGraphicsView(aParent)
{
    setRenderHint(QPainter::Antialiasing, false);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setOptimizationFlags(QGraphicsView::DontAdjustForAntialiasing | DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    resize(QSize(640, 480));

    scene_ = 0;
    //value_ = 0;
    drawGraduation();
    setScene(scene_);

}

TimeLine::~TimeLine()
{
    delete scene_;
}

void
TimeLine::drawGraduation()
{
    if (0 == scene_) {
        scene_ = new QGraphicsScene();
    }

    if (0 != transform().dx()) {
        translate(-transform().dx(), 0);
        //scene_->removeItem()
    }

    int width = viewport()->width();
    //int height = viewport()->height();
    int gradCount = (width / 10) * 2;

    for (int i = 0; i < gradCount; i++) {
        QGraphicsLineItem *item = new QGraphicsLineItem();
        item->setPen(QPen(QColor(Qt::black)));
        QLineF line;
        line.setP1(QPointF(i * 10, 0));

        if (0 == i % 5)
            line.setP2(QPointF(i * 10, 15));
        else
            line.setP2(QPointF(i * 10, 10));
        item->setLine(line);

        scene_->addItem(item);
    }

    for (int i = 0; i < gradCount; i++) {
        QGraphicsLineItem *item = new QGraphicsLineItem();
        item->setPen(QPen(QColor(Qt::black)));
        QLineF line;
        line.setP1(QPointF(i * 10, -15));

        if (0 == i % 5)
            line.setP2(QPointF(i * 10, 0));
        else
            line.setP2(QPointF(i * 10, -5));
        item->setLine(line);

        scene_->addItem(item);
    }

    for (int i = 0; i < gradCount; i++) {
        QGraphicsLineItem *item = new QGraphicsLineItem();
        item->setPen(QPen(QColor(Qt::black)));
        QLineF line;
        line.setP1(QPointF(i * 10, 15));

        if (0 == i % 5)
            line.setP2(QPointF(i * 10, 30));
        else
            line.setP2(QPointF(i * 10, 25));
        item->setLine(line);

        scene_->addItem(item);
    }

    for (int i = 0; i < gradCount; i++) {
            QGraphicsLineItem *item = new QGraphicsLineItem();
            item->setPen(QPen(QColor(Qt::black)));
            QLineF line;
            line.setP1(QPointF(i * 10, 30));

            if (0 == i % 5)
                line.setP2(QPointF(i * 10, 45));
            else
                line.setP2(QPointF(i * 10, 40));
            item->setLine(line);

            scene_->addItem(item);
        }

    for (int i = 0; i < gradCount; i++) {
            QGraphicsLineItem *item = new QGraphicsLineItem();
            item->setPen(QPen(QColor(Qt::black)));
            QLineF line;
            line.setP1(QPointF(i * 10, 45));

            if (0 == i % 5)
                line.setP2(QPointF(i * 10, 60));
            else
                line.setP2(QPointF(i * 10, 55));
            item->setLine(line);

            scene_->addItem(item);
        }

    for (int i = 0; i < gradCount; i++) {
            QGraphicsLineItem *item = new QGraphicsLineItem();
            item->setPen(QPen(QColor(Qt::black)));
            QLineF line;
            line.setP1(QPointF(i * 10, 60));

            if (0 == i % 5)
                line.setP2(QPointF(i * 10, 75));
            else
                line.setP2(QPointF(i * 10, 70));
            item->setLine(line);

            scene_->addItem(item);
        }


}

void
TimeLine::followTimeline()
{
    if (0 < transform().dx() - prev_translate_)
        value_ += int(transform().dx() - prev_translate_) / 10;
}

void
TimeLine::mousePressEvent(
    QMouseEvent * anEvent
)
{
    //prev_value_ = value_;
    //prev_translate_ = transform().dx();
    QGraphicsView::mousePressEvent(anEvent);
}

void
TimeLine::mouseMoveEvent(
    QMouseEvent *anEvent
)
{
    //followTimeline();

    //if (transform().dx() == viewport()->width() / 2)
        //drawGraduation();
    QGraphicsView::mouseMoveEvent(anEvent);
}
