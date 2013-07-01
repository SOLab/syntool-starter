#include "placeboxwidget.h"

PlaceBoxWidget::PlaceBoxWidget(QString nameValue, Geometry::Type type, qint32 objectNumber, QWidget *parent) :
    QWidget(parent)
{
    _typeObject = type;
    _objectNumber = objectNumber;

    hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);

    QLabel* nameLbl = new QLabel(nameValue, this);
    hLayout->addWidget(nameLbl);

    showPosButton = new QPushButton(QIcon(":/icons/target.png"), "", this);
    showPosButton->setFixedSize(20, 20);
    showPosButton->setIconSize(QSize(12,12));
    showPosButton->setToolTip(tr("Go to object"));

    removeButton = new QPushButton(QIcon(":/icons/delete.png"), "", this);
    removeButton->setFixedSize(20, 20);
    removeButton->setIconSize(QSize(12,12));
    removeButton->setToolTip(tr("Remove object"));
    connect(removeButton, &QPushButton::clicked, this, &PlaceBoxWidget::removeSlot);

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
