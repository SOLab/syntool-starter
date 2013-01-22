#include "timeline.h"

int partCount = 50;
int helper_cursop_position = 31;
#include <qdebug.h>

TimeLine::TimeLine(QWidget *parent)
: QWidget(parent)

{
    // минимальное, максимальное, текущее показываемое мин-макс, текущее значение (на каком отрезке стоит), размеры курсора, флаги движения и обновления диапазона
    control_.minDate = QDateTime(QDate(1900, 1, 1), QTime(0, 0, 0));
    control_.currentDate = QDateTime(QDate().currentDate(), QTime().currentTime());
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

    connect(calendarButton, SIGNAL(clicked()), this, SLOT(setDate()));

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
    createBottomRect();
    createTopRect();

    QPainter painter(this);

    QPen pen(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
//    painter.save();


    int partCount = 50;
    int maxWidth = partCount * (width()/partCount);
    qDebug() << width();
    maxWidth = width() - 1;


    // рисуем значение текущее
    painter.drawText(0,16, control_.currentDate.date().toString("dd.MM.yyyy")+ \
                     " "+control_.currentDate.time().toString("hh:mm"));

    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawLine(maxWidth/2, height(), maxWidth/2, 0);
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
        qDebug() << control_.pos_;
        int dx = control_.pos_.x() - pe->pos().x();
        // 2 minutes in pixel + 60 seconds in minute
        control_.currentDate = (control_.currentDate.addSecs(2*60*dx));
        qDebug() << control_.currentDate.time();
        control_.pos_ = pe->pos();
        repaint();
    }
    else if (control_.moveWeek)
    {
        // вычисляем куда его тянет, меняем значение переменной
        qDebug() << control_.pos_;
        int dx = control_.pos_.x() - pe->pos().x();
        // 2 minutes in pixel + 60 seconds in minute + 7 days in week
        control_.currentDate = (control_.currentDate.addSecs(7*2*60*dx));
        qDebug() << control_.currentDate.time();
//        checkMouse();
        control_.pos_ = pe->pos();
        repaint();
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
        painter.drawText(halfWidth - curDayPixel + 2, height() - 26, getDayMonth(control_.currentDate));
    }

    if (halfWidth > nextDayPixel)
    {
        painter.drawLine(halfWidth + nextDayPixel, height() - 24, halfWidth + nextDayPixel, 0);
        painter.drawText(halfWidth + nextDayPixel + 2, height() - 26, getDayMonth(control_.currentDate.addDays(1)));
    }

    if (halfWidth > prevDayPixel)
    {
        painter.drawLine(halfWidth - prevDayPixel, height() - 24, halfWidth - prevDayPixel, 0);
        painter.drawText(halfWidth - prevDayPixel + 2, height() - 26, getDayMonth(control_.currentDate.addDays(-1)));
    }

    if (halfWidth > afterTomorrowDayPixel)
    {
        painter.drawLine(halfWidth + afterTomorrowDayPixel, height() - 24, halfWidth + afterTomorrowDayPixel, 0);
        painter.drawText(halfWidth + afterTomorrowDayPixel + 2, height() - 26, getDayMonth(control_.currentDate.addDays(2)));
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
    repaint();
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

// draw markers

    pen.setColor(Qt::black);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);

    qDebug() << "Day of week" << control_.currentDate.date().dayOfWeek();
    int curWeekPixel = (control_.currentDate.time().hour() * 30 + (control_.currentDate.date().dayOfWeek() - 1)*24*30 \
                        + control_.currentDate.time().minute() / 2) /7;

    int nextWeekPixel = (720 - curWeekPixel);

    int prevWeekPixel = (curWeekPixel + 720);

    int afterNextWeekPixel = (nextWeekPixel + 720);

    int halfWidth = width()/2;

    if (halfWidth > curWeekPixel)
    {
        painter.drawLine(halfWidth - curWeekPixel, height() - 24, halfWidth - curWeekPixel, height());
        painter.drawText(halfWidth - curWeekPixel + 2, height() - 6, getFirstDayOfWeekMonth(control_.currentDate));
    }

    if (halfWidth > nextWeekPixel)
    {
        painter.drawLine(halfWidth + nextWeekPixel, height() - 24, halfWidth + nextWeekPixel, height());
        painter.drawText(halfWidth + nextWeekPixel + 2, height() - 6, getFirstDayOfWeekMonth(control_.currentDate.addDays(7)));
    }

    if (halfWidth > prevWeekPixel)
    {
        painter.drawLine(halfWidth - prevWeekPixel, height() - 24, halfWidth - prevWeekPixel, height());
        painter.drawText(halfWidth - prevWeekPixel + 2, height() - 6, getFirstDayOfWeekMonth(control_.currentDate.addDays(-7)));
    }

    if (halfWidth > afterNextWeekPixel)
    {
        painter.drawLine(halfWidth + afterNextWeekPixel, height() - 24, halfWidth + afterNextWeekPixel, height());
        painter.drawText(halfWidth + afterNextWeekPixel + 2, height() - 6, getFirstDayOfWeekMonth(control_.currentDate.addDays(14)));
    }

}
