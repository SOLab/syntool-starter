#ifndef TIMELINE_H
#define TIMELINE_H

#include <QtCore/qtimeline.h>
#include <QGraphicsProxyWidget>

class CustomProxy : public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    CustomProxy(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);

    QRectF boundingRect() const;
    void paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option,
                          QWidget *widget);
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private slots:
    void updateStep(qreal step);
    void stateChanged(QTimeLine::State);
    void zoomIn();
    void zoomOut();

private:
    QTimeLine *timeLine;
    bool popupShown;
    QGraphicsItem *currentPopup;
};

#endif // TIMELINE_H
