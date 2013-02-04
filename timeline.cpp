#include "timeline.h"

int partCount = 50;
int helper_cursop_position = 31;
#include <qdebug.h>

TimeLine::TimeLine(QWidget *parent)
: QWidget(parent)

{
    control_.minDate = QDateTime(QDate(1900, 1, 1), QTime(0, 0, 0));
    control_.currentDate = QDateTime(QDate().currentDate(), QTime().currentTime());
    notChangedDate = control_.currentDate.date();
    control_.maxDate = control_.currentDate;

    control_.markerDistance = 720;
    control_.dayRect = QRect();
    control_.weekRect = QRect();
    control_.moveWeek = false;
    control_.moveDay = false;
    control_.flagMovedIntervalos_ = true;
    // создаём таймер, раз в полсекунды он разрешает сдвигать диапазон. (чтоб полоса не ускакала на горку)
//    QTimer * timer = new QTimer;
//    timer->setInterval(500);
//    connect( timer, SIGNAL(timeout()), SLOT( moveEnabled()));
//    timer->start();
    setContentsMargins(0,0,0,0);

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

    connect(calendarButton, SIGNAL(clicked()), this, SLOT(setDate()));

    imageGeoPoint = QImage(":/orange-circle.png");

    hLayout->addWidget(calendarButton, 0, Qt::AlignRight | Qt::AlignTop);
}

TimeLine::~TimeLine()
{
}

void TimeLine::paintEvent(QPaintEvent * pe)
{
    Q_UNUSED(pe);

    control_.dayRect.setCoords(0, 0, width(), height() - 24);
    control_.weekRect.setCoords(0, height() - 24, width(), height());
//    painter.end();
    createTopRect();
    createBottomRect();

    QPainter painter(this);

    QPen pen(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
//    painter.save();


    int partCount = 50;
    int maxWidth = partCount * (width()/partCount);
//    qDebug() << width();
    maxWidth = width() - 1;


    // рисуем значение текущее
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

void TimeLine::setObjectsPointer(GetGranules* _getGranulesPointer)
{
    getGranulesPointer = _getGranulesPointer;
}

// нажатие мыши
void TimeLine::mousePressEvent ( QMouseEvent * pe )
{
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

// движение мыши.
void TimeLine::mouseMoveEvent(QMouseEvent * pe)
{
    // если флаг на движение стоит(попали мышкой в нужное поле и тянем)
    if (control_.moveDay)
    {
        // вычисляем куда его тянет, меняем значение переменной
//        qDebug() << control_.pos_;
        int dx = control_.pos_.x() - pe->pos().x();
        // 2 minutes in pixel + 60 seconds in minute
        control_.currentDate = (control_.currentDate.addSecs(2*60*dx));
//        qDebug() << control_.currentDate.time();
        control_.pos_ = pe->pos();
        repaint();
    }
    else if (control_.moveWeek)
    {
        // вычисляем куда его тянет, меняем значение переменной
//        qDebug() << control_.pos_;
        int dx = control_.pos_.x() - pe->pos().x();
        // 2 minutes in pixel + 60 seconds in minute + 7 days in week
        control_.currentDate = (control_.currentDate.addSecs(7*2*60*dx));
//        qDebug() << control_.currentDate.time();
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

    // отжатие - снимаем флаг.
void TimeLine::mouseReleaseEvent ( QMouseEvent * pe )
{
    control_.moveDay = false;
    control_.moveWeek = false;
    QWidget::mouseReleaseEvent (pe);
}


QString getDayMonth(QDateTime dateTime)
{
    return QString::number(dateTime.date().day()) + dateTime.date().shortMonthName(dateTime.date().month());
}

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
    painter.drawRect(0, 0, width(), height() - 24);

    // create dotline for days
    pen.setColor(Qt::black);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);

    int curDayPixel = control_.currentDate.time().hour() * 30 + control_.currentDate.time().minute() / 2;

    int prevDayPixel = curDayPixel + 720;

    int nextDayPixel = 720 - curDayPixel;

    int afterTomorrowDayPixel = nextDayPixel + 720;

    int halfWidth = width()/2;

    if (halfWidth > curDayPixel)
    {
        painter.drawLine(halfWidth - curDayPixel, height() - 24, halfWidth - curDayPixel, 0);
        painter.drawText(halfWidth - curDayPixel + 2, height() - 28, getDayMonth(control_.currentDate));
    }

    if (halfWidth > nextDayPixel)
    {
        painter.drawLine(halfWidth + nextDayPixel, height() - 24, halfWidth + nextDayPixel, 0);
        painter.drawText(halfWidth + nextDayPixel + 2, height() - 28,
                         getDayMonth(control_.currentDate.addDays(1)));
    }

    if (halfWidth > prevDayPixel)
    {
        painter.drawLine(halfWidth - prevDayPixel, height() - 24, halfWidth - prevDayPixel, 0);
        painter.drawText(halfWidth - prevDayPixel + 2, height() - 28,
                         getDayMonth(control_.currentDate.addDays(-1)));
    }

    if (halfWidth > afterTomorrowDayPixel)
    {
        painter.drawLine(halfWidth + afterTomorrowDayPixel, height() - 24, halfWidth + afterTomorrowDayPixel, 0);
        painter.drawText(halfWidth + afterTomorrowDayPixel + 2, height() - 28,
                         getDayMonth(control_.currentDate.addDays(2)));
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
    int curWeekPixel = (control_.currentDate.time().hour() * 30 + (control_.currentDate.date().dayOfWeek() - 1)*24*30 \
                        + control_.currentDate.time().minute() / 2) /7;

    int nextWeekPixel = (720 - curWeekPixel);

    int prevWeekPixel = (curWeekPixel + 720);

    int afterNextWeekPixel = (nextWeekPixel + 720);

    int halfWidth = width()/2;

    if (halfWidth > curWeekPixel)
    {
        painter.drawLine(halfWidth - curWeekPixel, height() - 24, halfWidth - curWeekPixel, height());
        painter.drawText(halfWidth - curWeekPixel + 2, height(), getFirstDayOfWeekMonth(control_.currentDate));
    }

    if (halfWidth > nextWeekPixel)
    {
        painter.drawLine(halfWidth + nextWeekPixel, height() - 24, halfWidth + nextWeekPixel, height());
        painter.drawText(halfWidth + nextWeekPixel + 2, height(),
                         getFirstDayOfWeekMonth(control_.currentDate.addDays(7)));
    }

    if (halfWidth > prevWeekPixel)
    {
        painter.drawLine(halfWidth - prevWeekPixel, height() - 24, halfWidth - prevWeekPixel, height());
        painter.drawText(halfWidth - prevWeekPixel + 2, height(),
                         getFirstDayOfWeekMonth(control_.currentDate.addDays(-7)));
    }

    if (halfWidth > afterNextWeekPixel)
    {
        painter.drawLine(halfWidth + afterNextWeekPixel, height() - 24, halfWidth + afterNextWeekPixel, height());
        painter.drawText(halfWidth + afterNextWeekPixel + 2, height(),
                         getFirstDayOfWeekMonth(control_.currentDate.addDays(14)));
    }

//    addGeoSegment(QDateTime().currentDateTime().addDays(-1), QDateTime().currentDateTime().addSecs(4444), 20, 20);
//    addGeoPoint(QDateTime().currentDateTime().addDays(-1).addSecs(-4444), 20, 20);

    drawAllMarkers();
}

void TimeLine::drawAllMarkers()
{
    QHash<QString, Granule>::const_iterator k = granulesHash->constBegin();
    while ( k != granulesHash->constEnd() )
    {
        addGeoPoint(k.value().startDate, 20,20);
        ++k;
    }

}

void TimeLine::setDate()
{
    calendar = new Calendar;
    connect(calendar->okButton, SIGNAL(clicked()), this, SLOT(setCurrentDate()));
    calendar->show();
}

void TimeLine::setCurrentDate()
{
    control_.currentDate.setDate(calendar->calendar->selectedDate());
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

void TimeLine::addGeoPoint(QDateTime dateTime, float lat, float lon)
{
    geoPoint newPoint = {dateTime, lat, lon};
    geoPointList.append(newPoint);
//    qDebug() << dateTime;


    qint64 secsTo = control_.currentDate.secsTo(dateTime);
    // secsTo/(2 * 60.0) = pixels
    qint64 pixels = secsTo/(120.0);

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
        // -5 because imege size 10x10
        painter.drawImage(width()/2+pixels - 5,20,imageGeoPoint);
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
        painter.drawLine(width()/2+weekPixelsToStart, height() - 15, width()/2+weekPixelsToStart, height() - 5);
        painter.drawLine(width()/2+weekPixelsToEnd, height() - 15, width()/2+weekPixelsToEnd, height() - 5);
        painter.drawLine(width()/2+weekPixelsToStart, height() - 5, width()/2+weekPixelsToEnd, height() - 5);
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
    emit getNewAllGranules();

}
