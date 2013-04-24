#include "timeline.h"

int partCount = 50;
int helper_cursop_position = 31;
#include <qdebug.h>

TimeLine::TimeLine(ConfigData configData, QWidget *parent)
: QWidget(parent)

{
    control_.minDate = QDateTime(QDate(1900, 1, 1), QTime(0, 0, 0));
    control_.currentDate = QDateTime(QDate().currentDate(), QTime().currentTime());
    notChangedDate = control_.currentDate.date();
    control_.maxDate = control_.currentDate;

    control_.markerDistance = 720;
    hourPixels = control_.markerDistance / 24;
    minutePixels = (24 * 60) / float(control_.markerDistance);
    secondsInPixel = 60 * minutePixels;

    control_.dayRect = QRect();
    control_.weekRect = QRect();
    control_.moveWeek = false;
    control_.moveDay = false;
    control_.flagMovedIntervalos_ = true;

    // Granule statuses
    GranuleStatuses << "New" << "Ingesting" << "Completed" << "Error" << "Ingested"
                  << "Processing" << "WaitingForPostProcessing";

    setContentsMargins(0,0,0,0);
    // <granuleId, granuleRectCoords>
    rectsGranules = new QHash<qint32, QRect>;
    currentGranuleId = 0;
    serverName = configData.serverName;

    QHBoxLayout* hLayout = new QHBoxLayout(this);
    QPushButton* calendarButton = new QPushButton("Set date", this);
    calendarButton->setFixedSize(60, 24);
    calendarButton->setStyleSheet(" QPushButton {\
                                        border: 1px solid #8f8f91; border-radius: 10px; \
                                        background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                                                                          stop: 0 #f6f7fa, stop: 1 #dadbde);\
                                        min-width: 80px;}\
                                    QPushButton:pressed {\
                                        background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                                                                          stop: 0 #dadbde, stop: 1 #f6f7fa);}");

    connect(calendarButton, &QPushButton::clicked, this, &TimeLine::setDate);

    imageGeoPoint = QImage(":/orange-circle.png");

    hLayout->addWidget(calendarButton, 0, Qt::AlignRight | Qt::AlignTop);
    createGranulesContextMenu();
}

TimeLine::~TimeLine()
{
}

// right click on the granule
void TimeLine::createGranulesContextMenu()
{
    granulesContextMenu = new QMenu(this);

    QAction *actionImage = new QAction(QString::fromUtf8("View image"), this);
    connect(actionImage, &QAction::triggered, this, &TimeLine::actionImageSlot);
    granulesContextMenu->addAction(actionImage);

    QAction *actionOpendap = new QAction(QString::fromUtf8("Open in OPeNDAP"), this);
    connect(actionOpendap, &QAction::triggered, this, &TimeLine::actionOpendapSlot);
    granulesContextMenu->addAction(actionOpendap);

    QAction *actionFtp = new QAction(QString::fromUtf8("Download"), this);
    connect(actionFtp, &QAction::triggered, this, &TimeLine::actionFtpSlot);
    granulesContextMenu->addAction(actionFtp);

    QAction *actionKml = new QAction(QString::fromUtf8("Get KML"), this);
    connect(actionKml, &QAction::triggered, this, &TimeLine::actionKmlSlot);
    granulesContextMenu->addAction(actionKml);
    granulesContextMenu->addSeparator();

    QAction *actionProperties = new QAction(QString::fromUtf8("Properties"), this);
    connect(actionProperties, &QAction::triggered, this, &TimeLine::actionPropertiesSlot);
    granulesContextMenu->addAction(actionProperties);
}

void TimeLine::actionImageSlot() {
    qint8 status = granulesHash->value(QString::number(currentGranuleId)).status;
    if (status != 2)
    {
        QMessageBox* msgBox = new QMessageBox(/*this*/);

        msgBox->setText("Granule status: " + QString(GranuleStatuses.at(status)));
        msgBox->exec();
        return;
    }

    granuleActions(serverName, QString::number(currentGranuleId), "image");
}

void TimeLine::actionOpendapSlot() {
    granuleActions(serverName, QString::number(currentGranuleId), "opendap");
}

void TimeLine::actionFtpSlot() {
    granuleActions(serverName, QString::number(currentGranuleId), "ftp");
}

void TimeLine::actionKmlSlot() {
    qint8 status = granulesHash->value(QString::number(currentGranuleId)).status;
    if (status != 2 && status != 6)
    {
        QMessageBox* msgBox = new QMessageBox(/*this*/);

        msgBox->setText("Granule status: " + QString(GranuleStatuses.at(status)));
        msgBox->exec();
        return;
    }

    granuleActions(serverName, QString::number(currentGranuleId), "kml");
}

void TimeLine::actionPropertiesSlot() {
    GranuleInfoWidget* currentGranuleWidget =
            new GranuleInfoWidget(granulesHash->value(QString::number(currentGranuleId)));
    currentGranuleWidget->show();
}

void TimeLine::paintEvent(QPaintEvent * pe)
{
    Q_UNUSED(pe);

    control_.dayRect.setCoords(0, 0, width(), height() - 24);
    control_.weekRect.setCoords(0, height() - 24, width(), height());

    // draw day and week rects (with markers and dates)
    createTopRect();
    createBottomRect();

    drawAllMarkers();

    QPainter painter(this);

    QPen pen(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);

    int partCount = 50;
    int maxWidth = partCount * (width()/partCount);

    maxWidth = width() - 1;

    // Draw current date and time
    painter.drawText(0,16, control_.currentDate.date().toString("dd.MM.yyyy")+ \
                     " "+control_.currentDate.time().toString("hh:mm"));

    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawLine(maxWidth/2, height(), maxWidth/2, 0);
}

void TimeLine::setSelectedProducts(QHash<QString, selectedProduct> *_selectedProducts,
                                   QHash<QString, Granule>* _granulesHash)
{
    selectedProducts = _selectedProducts;
    granulesHash = _granulesHash;
}

// press mouse
void TimeLine::mousePressEvent ( QMouseEvent * pe )
{
    // iterate all granules
    QHash<qint32, QRect>::const_iterator i = rectsGranules->constBegin();
    while (i != rectsGranules->constEnd()) {
        // if user hit in the granule
        if (i.value().contains(pe->pos()))
        {
            // selected granule
            currentGranuleId = i.key();

            // left or right button
            if (pe->button() == Qt::LeftButton)
                granulePressLeft();
            else if(pe->button() == Qt::RightButton)
                granulePressRight();

            QWidget::mousePressEvent(pe);
            return;
        }
        ++i;
    }

    // if user hit in the day or week rectangle
    if (control_.dayRect.contains(pe->pos()))
        control_.moveDay = true;
    else if (control_.weekRect.contains(pe->pos()))
        control_.moveWeek = true;
    else
    {
        control_.moveDay = false;
        control_.moveWeek = false;
    }
    control_.pos_ = pe->pos();
    QWidget::mousePressEvent(pe);
}

void TimeLine::granulePressLeft()
{
    GetGranuleCoords* getCoords = new GetGranuleCoords(this);
    connect (getCoords, SIGNAL(coordsSignal(qint32,float,float,float,float)),
             this, SLOT(setCoordsGranule(qint32,float,float,float,float)));
    getCoords->getCoords(serverName, currentGranuleId);
    qDebug() << "granulePressLeft";
}

void TimeLine::setCoordsGranule(qint32 granuleId, float north, float east, float south, float west)
{
    Granule granule= granulesHash->value(QString::number(granuleId));
    granule.north = north;
    granule.east = east;
    granule.south = south;
    granule.west = west;

    granule.centerLat = (north + south) / 2.0;
    granule.centerLon = (east + west) / 2.0;

    granulesHash->insert(QString::number(granuleId), granule);

    emit moveEarth(granule.centerLat, granule.centerLon);
}

void TimeLine::granulePressRight()
{
    qDebug() << "granulePressRight";
    // show context menu
    granulesContextMenu->popup(QCursor::pos());
}

// move mouse
void TimeLine::mouseMoveEvent(QMouseEvent * pe)
{
    // если флаг на движение стоит(попали мышкой в нужное поле и тянем)
    if (control_.moveDay)
    {
        // вычисляем куда его тянет, меняем значение переменной
//        qDebug() << control_.pos_;
        int dx = control_.pos_.x() - pe->pos().x();

        // 2 minutes in pixel + 60 seconds in minute
        control_.currentDate = (control_.currentDate.addSecs(secondsInPixel*dx));
        control_.pos_ = pe->pos();
        repaint();
    }
    else if (control_.moveWeek)
    {
        // вычисляем куда его тянет, меняем значение переменной
        int dx = control_.pos_.x() - pe->pos().x();

        // 2 minutes in pixel + 60 seconds in minute + 7 days in week
        control_.currentDate = (control_.currentDate.addSecs(7*secondsInPixel*dx));
        control_.pos_ = pe->pos();
        repaint();
    }

    qint64 daysDiff = notChangedDate.daysTo(control_.currentDate.date());
    if (qAbs(daysDiff) > 2)
    {
        notChangedDate = control_.currentDate.date();
        changedDay();
    }

    QWidget::mouseMoveEvent(pe);
}

// release - remove the flags
void TimeLine::mouseReleaseEvent ( QMouseEvent * pe )
{
    control_.moveDay = false;
    control_.moveWeek = false;
    QWidget::mouseReleaseEvent (pe);
}

void TimeLine::wheelEvent(QWheelEvent *pe)
{
    if (pe->delta() < 0 && control_.markerDistance > 360)
    {
        control_.markerDistance -= 360;
        emit getNewAllGranules(control_.markerDistance/360);
    }
    else if (pe->delta() > 0 && control_.markerDistance < 1440)
    {
        control_.markerDistance += 360;
//        emit getNewAllGranules(control_.markerDistance/360);
    }

    hourPixels = control_.markerDistance / 24;
    minutePixels = (24 * 60) / float(control_.markerDistance);
    secondsInPixel = 60 * minutePixels;
    update();
}

// get current QString dayMonth (ex. 14feb.)
QString getDayMonth(QDateTime dateTime)
{
    return QString::number(dateTime.date().day()) + dateTime.date().shortMonthName(dateTime.date().month());
}

// get first day of the week QString dayMonth (ex. 14feb.)
QString getFirstDayOfWeekMonth(QDateTime dateTime)
{
    QDateTime tempDate = dateTime.addDays(-1*dateTime.date().dayOfWeek() + 1);
    return QString::number(tempDate.date().day()) + tempDate.date().shortMonthName(tempDate.date().month());
}

void TimeLine::createTopRect()
{
    QPainter painter(this);

    QBrush brush(QColor(238,238,238));
    painter.setBrush(brush);
    QPen pen(QColor(238,238,238));
    pen.setWidth(1);

    painter.setPen(pen);
    // draw main rectangle
    painter.drawRect(0, 0, width(), height() - 24);

    // create line style (dotline) for days
    pen.setColor(Qt::black);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);

    // calculate count pixels for every day
    int curDayPixel = control_.currentDate.time().hour() * hourPixels
                      + control_.currentDate.time().minute() / minutePixels;

    int prevDayPixel = curDayPixel + control_.markerDistance;

    int nextDayPixel = control_.markerDistance - curDayPixel;

    // draw center (red) line
    int halfWidth = width()/2;

    // draw days
    if (halfWidth > curDayPixel)
    {
        painter.drawLine(halfWidth - curDayPixel, height() - 24, halfWidth - curDayPixel, 0);
        painter.drawText(halfWidth - curDayPixel + 2, height() - 28, getDayMonth(control_.currentDate));
    }

    int dayForText = -1;
    while (halfWidth > prevDayPixel)
    {
        painter.drawLine(halfWidth - prevDayPixel, height() - 24, halfWidth - prevDayPixel, 0);
        painter.drawText(halfWidth - prevDayPixel + 2, height() - 28,
                         getDayMonth(control_.currentDate.addDays(dayForText)));
        prevDayPixel += control_.markerDistance;
        dayForText -= 1;
    }

    dayForText = 1;
    while (halfWidth > nextDayPixel)
    {
        // -2 for don't shift image
        painter.drawLine(halfWidth + nextDayPixel - 2, height() - 24, halfWidth + nextDayPixel - 2, 0);
        painter.drawText(halfWidth + nextDayPixel + 2, height() - 28,
                         getDayMonth(control_.currentDate.addDays(dayForText)));

        nextDayPixel += control_.markerDistance;
        dayForText += 1;
    }
}

void TimeLine::createBottomRect()
{
    QPainter painter(this);

    QBrush brush(QColor(221,221,221));
    painter.setBrush(brush);
    QPen pen(QColor(221,221,221));
    pen.setWidth(1);

    painter.setPen(pen);
    painter.drawRect(0, height() - 24, width(), height());

    // draw current section
    painter.setBrush(QBrush(QColor(241,241,241)));
    float width_part = width()/7.0;

    painter.drawRect(qFloor(width_part*3 + 0.5), height() - 22, qFloor(width_part + 0.5), height());

    // draw markers
    pen.setColor(Qt::black);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);

//    qDebug() << "Day of week" << control_.currentDate.date().dayOfWeek();

    // calculate count pixels for every week
    int curWeekPixel = (control_.currentDate.time().hour() * hourPixels +
                        (control_.currentDate.date().dayOfWeek() - 1)*24*hourPixels
                        + control_.currentDate.time().minute() / minutePixels) /7;

    int nextWeekPixel = (control_.markerDistance - curWeekPixel);

    int prevWeekPixel = (curWeekPixel + control_.markerDistance);

//    int afterNextWeekPixel = (nextWeekPixel + control_.markerDistance);

    int halfWidth = width()/2;

    if (halfWidth > curWeekPixel)
    {
        painter.drawLine(halfWidth - curWeekPixel, height() - 24, halfWidth - curWeekPixel, height());
        painter.drawText(halfWidth - curWeekPixel + 2, height(), getFirstDayOfWeekMonth(control_.currentDate));
    }

    int dayForText = -7;
    while (halfWidth > prevWeekPixel)
    {
        painter.drawLine(halfWidth - prevWeekPixel, height() - 24, halfWidth - prevWeekPixel, height());
        painter.drawText(halfWidth - prevWeekPixel + 2, height(),
                         getFirstDayOfWeekMonth(control_.currentDate.addDays(dayForText)));
        prevWeekPixel += control_.markerDistance;
        dayForText -= 7;
    }

    dayForText = 7;
    while (halfWidth > nextWeekPixel)
    {
        painter.drawLine(halfWidth + nextWeekPixel, height() - 24, halfWidth + nextWeekPixel, height());
        painter.drawText(halfWidth + nextWeekPixel + 2, height(),
                         getFirstDayOfWeekMonth(control_.currentDate.addDays(dayForText)));

        nextWeekPixel += control_.markerDistance;
        dayForText += 7;
    }

//    addGeoSegment(QDateTime().currentDateTime().addDays(-1), QDateTime().currentDateTime().addSecs(4444), 20, 20);
//    addGeoPoint(QDateTime().currentDateTime().addDays(-1).addSecs(-4444), 20, 20);
}

void TimeLine::drawAllMarkers()
{
    rectsGranules->clear();
    displayedGranules.clear();
    QHash<QString, Granule>::const_iterator k = granulesHash->constBegin();
    while ( k != granulesHash->constEnd() )
    {
        addGeoPoint(k.value().startDate, k.value().granuleId, 20,20, k.value().productId);
        ++k;
    }
    emit changedDisplayGranules(displayedGranules);
//    qDebug() << "Count Granules = " << rectsGranules->count();

}

void TimeLine::addGeoPoint(QDateTime dateTime, qint32 granuleId, float lat, float lon, int productId)
{
    geoPoint newPoint = {dateTime, lat, lon};
    geoPointList.append(newPoint);
//    qDebug() << dateTime;

    qint64 secsTo = control_.currentDate.secsTo(dateTime);
    // secsTo/(2 * 60.0) = pixels
    qint64 pixels = secsTo/secondsInPixel;

    // Draw in week box
    int weekPixels = pixels/7;
    if (qAbs(weekPixels) <= width()/2)
    {
        QPainter painter(this);
        QPen pen = painter.pen();
        pen.setColor(QColor(255, 100, 100));
        pen.setWidth(2);
        painter.setPen(pen);
        // -5 because imege size 10x10
        painter.drawLine(width()/2+weekPixels, height() - 20, width()/2+weekPixels, height() - 10);
    }
    else
    {
        return;
    }

    // draw in day box
    if (qAbs(pixels) <= width()/2)
    {
        QPainter painter(this);

        if (productId > 50)
        {
            productId -= 50;
        }

        // -5 because imege size 10x10
        painter.drawImage(width()/2+pixels - 5,10 + productId,imageGeoPoint);
        rectsGranules->insert(granuleId, QRect(width()/2+pixels - 5, 10 + productId, 10, 10));
        displayedGranules.append(granuleId);
    }
}

void TimeLine::addGeoSegment(QDateTime startDateTime, QDateTime endDateTime, float lat, float lon)
{
    geoSegment newPoint = {startDateTime, endDateTime, lat, lon};
    geoSegmentList.append(newPoint);

    qint64 secsToStart = control_.currentDate.secsTo(startDateTime);
    qint64 secsToEnd = control_.currentDate.secsTo(endDateTime);

    qint64 pixelsToStart = secsToStart/(120.0);
    qint64 pixelsToEnd = secsToEnd/(120.0);

    // Draw in week box
    int weekPixelsToStart = pixelsToStart/7;
    int weekPixelsToEnd = pixelsToEnd/7;
    if ((qAbs(weekPixelsToStart) <= width()/2 || qAbs(weekPixelsToEnd) <= width()/2) \
            || (weekPixelsToStart < 0 && weekPixelsToEnd > 0))
    {
        QPainter painter(this);
        QPen pen = painter.pen();
        pen.setColor(QColor(196, 118, 0));
        pen.setWidth(1);
        painter.setPen(pen);

        // left, right and bottom side
        painter.drawLine(width()/2+weekPixelsToStart, height() - 15,
                         width()/2+weekPixelsToStart, height() - 5);
        painter.drawLine(width()/2+weekPixelsToEnd, height() - 15,
                         width()/2+weekPixelsToEnd, height() - 5);
        painter.drawLine(width()/2+weekPixelsToStart, height() - 5,
                         width()/2+weekPixelsToEnd, height() - 5);
    }
    else
    {
        return;
    }

    // Draw in day box
    if ((qAbs(pixelsToStart) <= width()/2 || qAbs(pixelsToEnd) <= width()/2) \
            || (pixelsToStart < 0 && secsToEnd > 0))
    {
        QPainter painter(this);
        QPen pen;
        pen.setWidth(1);
        QColor firstColor(246, 162, 0);
        QColor secondColor(196, 118, 0);

        QLinearGradient gradient(0, 0, 500, 100);
        gradient.setColorAt(0, firstColor);
        gradient.setColorAt(1, secondColor);
        pen.setBrush(gradient);
        painter.setPen(pen);
        painter.setBrush(gradient);

        painter.drawRect(width()/2+pixelsToStart, 30, pixelsToEnd-pixelsToStart, 8);
    }
}

void TimeLine::setDate()
{
    calendar = new Calendar(control_.currentDate.date());
    connect(calendar, &Calendar::okClicked, this, &TimeLine::setCurrentDate);
    calendar->show();
}

void TimeLine::setCurrentDate()
{
    qWarning() << calendar->calendar->selectedDate().toString();
    control_.currentDate.setDate(calendar->calendar->selectedDate());
    qWarning() << control_.currentDate.toString();
    calendar->close();
    calendar->deleteLater();

    qint64 daysDiff = notChangedDate.daysTo(control_.currentDate.date());
    if (qAbs(daysDiff) > 3)
    {
        notChangedDate = control_.currentDate.date();
        changedDay();
    }
//    repaint();
}

void TimeLine::changedDay()
{
//    qDebug() << "changedDay";

    // Remove old granules from cache
    QList<QString> granuleIdlist;

    QHash<QString, Granule>::const_iterator k = granulesHash->constBegin();
    while ( k != granulesHash->constEnd())
    {
        qint64 daysDiff = k.value().startDate.daysTo(control_.currentDate);
        if (qAbs(daysDiff) > 10)
        {
//            qDebug() << "REMOVE GRANULES";
            granuleIdlist.append(k.key());
        }
        ++k;
    }

    QList<QString>::iterator cur = granuleIdlist.begin();
    QList<QString>::iterator last = granuleIdlist.end();

    while ( cur != last)
    {
        qDebug() << "REMOVE GRANULES: " << granuleIdlist.at(0);
        granulesHash->remove(granuleIdlist.takeFirst());
        cur = granuleIdlist.begin();
        last = granuleIdlist.end();
    };

    // get new granules for all products
    emit getNewAllGranules(control_.markerDistance/360);

}
