#include "placeboxwidget.h"

PlaceBoxWidget::PlaceBoxWidget(QString nameValue, Geometry::Type type, qint32 objectNumber, QWidget *parent) :
    QWidget(parent)
{
    _typeObject = type;
    _objectNumber = objectNumber;

    hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);

    nameLbl = new QLineEdit(nameValue, this);
    nameLbl->setReadOnly(true);
    _style = "QLineEdit{border: red;"
            "background-color: "+
            QString("rgb(%1, %2, %3);}")
            .arg(parent->palette().background().color().red())
            .arg(parent->palette().background().color().green())
            .arg(parent->palette().background().color().blue());
    nameLbl->setStyleSheet(_style);

    hLayout->addWidget(nameLbl);

    showPosButton = new QPushButton(QIcon(":/icons/target.png"), "", this);
    showPosButton->setFixedSize(20, 20);
    showPosButton->setIconSize(QSize(12,12));
    showPosButton->setToolTip(tr("Go to object"));
    connect(showPosButton, &QPushButton::clicked, this, &PlaceBoxWidget::moveToObject);

    removeButton = new QPushButton(QIcon(":/icons/delete.png"), "", this);
    removeButton->setFixedSize(20, 20);
    removeButton->setIconSize(QSize(12,12));
    removeButton->setToolTip(tr("Remove object"));
    connect(removeButton, &QPushButton::clicked, this, &PlaceBoxWidget::removeSlot);

    renameButton = new QPushButton(QIcon(":/icons/pencil.png"), "", this);
    renameButton->setFixedSize(20, 20);
    renameButton->setIconSize(QSize(12,12));
    renameButton->setToolTip(tr("Rename"));
    renameButton->setCheckable(true);
    connect(renameButton, &QPushButton::clicked, this, &PlaceBoxWidget::renameSlot);

    hLayout->addWidget(renameButton);
    hLayout->addWidget(showPosButton);
    hLayout->addWidget(removeButton);

    resize(sizeHint());
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void PlaceBoxWidget::setCoordinates(GeoCoords pos1, GeoCoords pos2)
{
    _pos1 = pos1;
    _pos2 = pos2;
}

void PlaceBoxWidget::removeSlot()
{
    emit removeSignal(_typeObject, _objectNumber);
}

void PlaceBoxWidget::moveToObject()
{
    GeoCoords pos;
    if (_typeObject == Geometry::Line || _typeObject == Geometry::Rect)
    {
        GeoCoords pos2 = _pos2;

        if (qAbs(_pos2.lon - _pos1.lon) > M_PI)
        {
            if (pos2.lon > 0)
                pos2.lon -= 2*M_PI;
            else
                pos2.lon += 2*M_PI;
        }

        pos.lat = (_pos1.lat + _pos2.lat)/2.0;
        pos.lon = (_pos1.lon + pos2.lon)/2.0;
        if (pos.lon > M_PI)
            pos.lon -= 2*M_PI;
        else if (pos.lon < -M_PI)
            pos.lon += 2*M_PI;
    }
    else
        pos = _pos1;
    emit moveToObjectSignal(pos );
}

void PlaceBoxWidget::renameSlot(bool value)
{
    nameLbl->setReadOnly(!value);
    if (value)
    {
        nameLbl->setStyleSheet("");
        nameLbl->setFocus();
    }
    else
    {
        nameLbl->setStyleSheet(_style);
    }
}
