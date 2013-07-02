#include "placewidget.h"

PlaceWidget::PlaceWidget(QWidget *parent) :
    QWidget(parent)
{
    vLayout = new QVBoxLayout(this);
    vLayout->setAlignment(Qt::AlignTop);
    vLayout->setContentsMargins(0,0,0,0);

    // points
    pointLayout = new QVBoxLayout(this);
    pointLayout->setContentsMargins(0,0,0,0);

    QLabel* pointLbl = new QLabel(tr("Points")+":", this);
    pointLbl->setStyleSheet("QLabel {font-weight: bold;}");
    pointLayout->addWidget(pointLbl);

    // lines
    lineLayout = new QVBoxLayout(this);
    lineLayout->setContentsMargins(0,0,0,0);

    QLabel* lineLbl = new QLabel(tr("Lines")+":", this);
    lineLbl->setStyleSheet("QLabel {font-weight: bold;}");
    lineLayout->addWidget(lineLbl);

    // rectangles
    rectLayout = new QVBoxLayout(this);
    rectLayout->setContentsMargins(0,0,0,0);

    QLabel* rectLbl = new QLabel(tr("Rectangles")+":", this);
    rectLbl->setStyleSheet("QLabel {font-weight: bold;}");
    rectLayout->addWidget(rectLbl);

    vLayout->addLayout(pointLayout);
    vLayout->addLayout(lineLayout);
    vLayout->addLayout(rectLayout);

    setObjectName("PlaceWidget");
    setStyleSheet(QString("QWidget#PlaceWidget {background-color: "
                  "rgb(%1, %2, %3);}")
                  .arg(parent->palette().background().color().red())
                  .arg(parent->palette().background().color().green())
                  .arg(parent->palette().background().color().blue()));

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    tagPinMap = new QMap<qint32, PlaceBoxWidget*>();
    lineMap = new QMap<qint32, PlaceBoxWidget*>();
    rectMap = new QMap<qint32, PlaceBoxWidget*>();
}

void PlaceWidget::addPoint(qint32 pointNumber, GeoCoords pos, QString pointName)
{
    PlaceBoxWidget* pointWidget = new PlaceBoxWidget(pointName, Geometry::Point, pointNumber, this);
    GeoCoords pos2 = {-1000, -1000};
    pointWidget->setCoordinates(pos, pos2);

    connect(pointWidget, &PlaceBoxWidget::moveToObjectSignal, this, &PlaceWidget::moveToCoordsSignal);

    connect(pointWidget, &PlaceBoxWidget::removeSignal, this, &PlaceWidget::removeObjectSlot);
    connect(pointWidget, &PlaceBoxWidget::removeSignal, this, &PlaceWidget::removeObjectSignal);

    pointLayout->addWidget(pointWidget);
    tagPinMap->insert(pointNumber, pointWidget);
}

void PlaceWidget::addLine(qint32 lineNumber, GeoCoords pos1, GeoCoords pos2, QString lineName)
{
    PlaceBoxWidget* lineWidget = new PlaceBoxWidget(lineName, Geometry::Line, lineNumber, this);
    lineWidget->setCoordinates(pos1, pos2);

    connect(lineWidget, &PlaceBoxWidget::moveToObjectSignal, this, &PlaceWidget::moveToCoordsSignal);

    connect(lineWidget, &PlaceBoxWidget::removeSignal, this, &PlaceWidget::removeObjectSlot);
    connect(lineWidget, &PlaceBoxWidget::removeSignal, this, &PlaceWidget::removeObjectSignal);

    lineLayout->addWidget(lineWidget);
    lineMap->insert(lineNumber, lineWidget);
}

void PlaceWidget::addRect(qint32 rectNumber, GeoCoords pos1, GeoCoords pos2, QString rectName)
{
    PlaceBoxWidget* rectWidget = new PlaceBoxWidget(rectName, Geometry::Rect, rectNumber, this);
    rectWidget->setCoordinates(pos1, pos2);

    connect(rectWidget, &PlaceBoxWidget::moveToObjectSignal, this, &PlaceWidget::moveToCoordsSignal);

    connect(rectWidget, &PlaceBoxWidget::removeSignal, this, &PlaceWidget::removeObjectSlot);
    connect(rectWidget, &PlaceBoxWidget::removeSignal, this, &PlaceWidget::removeObjectSignal);

    rectLayout->addWidget(rectWidget);
    rectMap->insert(rectNumber, rectWidget);
}

void PlaceWidget::removeObjectSlot(Geometry::Type type, qint32 objectNumber)
{
    if (type == Geometry::Line)
    {
        PlaceBoxWidget* lineWidget = lineMap->value(objectNumber);
        lineMap->remove(objectNumber);
        if (lineWidget)
            lineWidget->close();
    }
    else if (type == Geometry::Rect)
    {
        PlaceBoxWidget* rectWidget = rectMap->value(objectNumber);
        rectMap->remove(objectNumber);
        if (rectWidget)
            rectWidget->close();
    }
    else if (type == Geometry::Pin || type == Geometry::Tag || type == Geometry::Point)
    {
        PlaceBoxWidget* pointWidget = tagPinMap->value(objectNumber);
        tagPinMap->remove(objectNumber);
        if (pointWidget)
            pointWidget->close();
    }
}

