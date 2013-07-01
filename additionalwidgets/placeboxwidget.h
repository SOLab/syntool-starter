#ifndef PLACEBOXWIDGET_H
#define PLACEBOXWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <qmath.h>

#include "more/structure.h"

class PlaceBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlaceBoxWidget(QString nameValue, Geometry::Type type, qint32 objectNumber, QWidget *parent = 0);
    void setCoordinates(GeoCoords pos1, GeoCoords pos2);

    QLineEdit* nameLbl;
    QHBoxLayout* hLayout;
    QPushButton* showPosButton;
    QPushButton* removeButton;
    QPushButton* renameButton;

private:
    QString _nameValue;
    qint32 _objectNumber;
    Geometry::Type _typeObject;
    GeoCoords _pos1;
    GeoCoords _pos2;

    QString _style;

signals:
    void removeSignal(Geometry::Type type, qint32 objectNumber);
    void moveToObjectSignal(GeoCoords pos);
    
public slots:
    void removeSlot();
    void moveToObject();
    void renameSlot(bool value);
};

#endif // PLACEBOXWIDGET_H
