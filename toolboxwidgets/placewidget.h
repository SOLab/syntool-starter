#ifndef PLACEWIDGET_H
#define PLACEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

#include "more/structure.h"
#include "additionalwidgets/placeboxwidget.h"

class PlaceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlaceWidget(QWidget *parent = 0);

protected:
    QVBoxLayout* vLayout;
    QVBoxLayout* pointLayout;
    QVBoxLayout* lineLayout;
    QVBoxLayout* rectLayout;

    QMap<qint32, PlaceBoxWidget*>* tagPinMap;
    QMap<qint32, PlaceBoxWidget*>* lineMap;
    QMap<qint32, PlaceBoxWidget*>* rectMap;

signals:
    void removeObjectSignal(Geometry::Type type, qint32 objectNumber);
    void moveToCoordsSignal(GeoCoords pos);
    
public slots:
    void addPoint(qint32 pointNumber, GeoCoords pos, QString pointName);
    void addLine(qint32 lineNumber, GeoCoords pos1, GeoCoords pos2, QString lineName);
    void addRect(qint32 rectNumber, GeoCoords pos1, GeoCoords pos2, QString rectName);

    void removeObject(Geometry::Type type, qint32 objectNumber);

};

#endif // PLACEWIDGET_H
