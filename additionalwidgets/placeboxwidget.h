#ifndef PLACEBOXWIDGET_H
#define PLACEBOXWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "more/structure.h"

class PlaceBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlaceBoxWidget(QString nameValue, Geometry::Type type, qint32 objectNumber, QWidget *parent = 0);
    void setCoordinates(GeoCoords pos1, GeoCoords pos2);

    QHBoxLayout* hLayout;
    QPushButton* showPosButton;
    QPushButton* removeButton;

private:
    QString _nameValue;
    qint32 _objectNumber;
    Geometry::Type _typeObject;
    GeoCoords _pos1;
    GeoCoords _pos2;

signals:
    void removeSignal(Geometry::Type type, qint32 objectNumber);
    
public slots:
    void removeSlot();
    
};

#endif // PLACEBOXWIDGET_H
