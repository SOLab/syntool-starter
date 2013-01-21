#include "timeline.h"

int partCount = 50;
int helper_cursop_position = 31;
#include <qdebug.h>

TimeLine::TimeLine(QWidget *parent)
: QWidget(parent)
//{
//    // минимальное, максимальное, текущее показываемое мин-макс, текущее значение (на каком отрезке стоит), размеры курсора, флаги движения и обновления диапазона
//    control_.min_ = 0;
//    control_.max_ = 200;
//    control_.curMax_ = 50;
//    control_.curMin_ = 0;
//    control_.current_ = 20;
//    QRect rect(0,0,10,18);
//    control_.cursorRect_ = rect;
//    control_.move_ = false;
//    control_.flagMovedIntervalos_ = true;
//    // создаём таймер, раз в полсекунды он разрешает сдвигать диапазон. (чтоб полоса не ускакала на горку)
//    QTimer * timer = new QTimer;
//    timer->setInterval(500);
//    connect( timer, SIGNAL(timeout()), SLOT( moveEnabled()));
//    timer->start();
//    setContentsMargins(0,0,0,0);
//}
{
    // минимальное, максимальное, текущее показываемое мин-макс, текущее значение (на каком отрезке стоит), размеры курсора, флаги движения и обновления диапазона
    control_.minDate = QDateTime(QDate(1900, 1, 1), QTime(0, 0, 0));
    control_.currentDate = QDateTime(QDate().currentDate(), QTime().currentTime());
    control_.maxDate = control_.currentDate;

    control_.markerDistance = 720;
//    control_.curMaxDate = 50;
//    control_.curMinDate = 0;
//    QRect rect(0,0,10,18);
//    control_.cursorRect_ = rect;
    control_.dayRect = QRect();
    control_.weekRect = QRect();
    control_.moveWeek = false;
    control_.moveDay = false;
    control_.flagMovedIntervalos_ = true;
    // создаём таймер, раз в полсекунды он разрешает сдвигать диапазон. (чтоб полоса не ускакала на горку)
    QTimer * timer = new QTimer;
    timer->setInterval(500);
    connect( timer, SIGNAL(timeout()), SLOT( moveEnabled()));
    timer->start();
    setContentsMargins(0,0,0,0);
}

TimeLine::~TimeLine()
{
}

void TimeLine::moveEnabled()
{
    control_.flagMovedIntervalos_ = true;
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
//    qDebug() << "==>" << maxWidth;
    qDebug() << width();
    maxWidth = width() - 1;


    // рисуем значение текущее
    painter.drawText(0,16, control_.currentDate.date().toString("dd.MM.yyyy")+ \
                     " "+control_.currentDate.time().toString("hh:mm"));
    // ручка чёрная, толщина 1, устанавливаем, сохраняем настройки.
//    QPen pen(Qt::black);
//    pen.setWidth(1);
//    painter.setPen(pen);
//    painter.save();
    // количество ячеек на линии, расчёт длины помещающихся отрезков,
    // целого числа(вот тут хз у меня чёто с формулой намудрено, но работает :D)

    // рисует рамку, рисуем основную линию, рисуем текст макс мин.
//    painter.drawRect(0, height()-50, maxWidth, 40);
//    painter.drawLine(0,height()-30,maxWidth,height()-30);
//    painter.drawText(0, height()-60, QString::number(control_.curMin_));
//    painter.drawText(maxWidth - 20, height()-60, QString::number(control_.curMax_));

    //  длина 1 отрезка, в цикле рисуем их дофига(50 штук, как и заказывали)
//    int widthOnePart = (width() - 3)/ float(partCount);
//    for (int i = 0; i <= partCount; i++)
//    {
//        painter.drawLine(widthOnePart*i, height()-40, widthOnePart*i, height()-20);
//    }

//    painter.end();

//    painter.begin(this);
    pen.setColor(Qt::red);
//    painter.begin(this);
    painter.setPen(pen);
//    painter.save();
    painter.drawLine(maxWidth/2, height(), maxWidth/2, 0);

//    painter.restore();
//    painter.end();
//    // проверка флага на движение мышкой
//    if (!control_.move_)
//    {
//        // получаем координаты курсора - текущее значение на длину 1 отрезка.
//        // Перемещаем наш курсор центром на это деление.
//        // Рисуем :) Так же рисуем линию от центра до курсора( дебажная вещь, можно закомментить.)
////        painter.drawLine(maxWidth/2, height(), maxWidth/2, 0);
////        painter.drawLine(maxWidth/2, height()-50, maxWidth/2, height()-10);
////        int tmpInt = (control_.current_) * widthOnePart;
////        control_.cursorRect_.moveCenter(QPoint(tmpInt,height()-helper_cursop_position));
////        painter.drawRect(control_.cursorRect_);
////		painter.drawLine(QPoint(width()/2,height()/2), control_.cursorRect_.center());
//    }
//    else
//    {
//        // проверка координат мыши, не даёт вылезти курсору за пределы нашего слайдера блин :)
//        int posX = control_.pos_.x();
//        if (posX > maxWidth)
//            posX = maxWidth;
//        if (posX < 0)
//            posX = 0;
//        // перемещаем под мышку, рисуем
//        control_.cursorRect_.moveCenter(QPoint(posX,height()-helper_cursop_position));
//        painter.drawRect(control_.cursorRect_);
//    }
//    // восстанавливаем настройки паинтера
//    painter.restore();
}


//void TimeLine::paintEvent(QPaintEvent * pe)
//{
//    Q_UNUSED(pe);
//    QPainter painter(this);
//    // рисуем значение текущее
//    painter.drawText(40,10, QString::number(control_.current_ + control_.curMin_));
//    // ручка чёрная, толщина 1, устанавливаем, сохраняем настройки.
//    QPen pen(Qt::black);
//    pen.setWidth(1);
//    painter.setPen(pen);
//    painter.save();
//    // количество ячеек на линии, расчёт длины помещающихся отрезков,
//    // целого числа(вот тут хз у меня чёто с формулой намудрено, но работает :D)
//    int partCount = 50;
//    int maxWidth = partCount * (width()/partCount);
//    qDebug() << "==>" << maxWidth;
//    qDebug() << width();
//    maxWidth = width() - 1;

//    // рисует рамку, рисуем основную линию, рисуем текст макс мин.
//    painter.drawRect(0, height()-50, maxWidth, 40);
//    painter.drawLine(0,height()-30,maxWidth,height()-30);
//    painter.drawText(0, height()-60, QString::number(control_.curMin_));
//    painter.drawText(maxWidth - 20, height()-60, QString::number(control_.curMax_));

//    //  длина 1 отрезка, в цикле рисуем их дофига(50 штук, как и заказывали)
//    int widthOnePart = (width() - 3)/ float(partCount);
//    for (int i = 0; i <= partCount; i++)
//    {
//        painter.drawLine(widthOnePart*i, height()-40, widthOnePart*i, height()-20);
//    }

//    QBrush brush(Qt::red);
//    painter.setBrush(brush);
////    painter.save();
//    // проверка флага на движение мышкой
//    if (!control_.move_)
//    {
//        // получаем координаты курсора - текущее значение на длину 1 отрезка.
//        // Перемещаем наш курсор центром на это деление.
//        // Рисуем :) Так же рисуем линию от центра до курсора( дебажная вещь, можно закомментить.)
//        int tmpInt = (control_.current_) * widthOnePart;
//        control_.cursorRect_.moveCenter(QPoint(tmpInt,height()-helper_cursop_position));
//        painter.drawRect(control_.cursorRect_);
////		painter.drawLine(QPoint(width()/2,height()/2), control_.cursorRect_.center());
//    }
//    else
//    {
//        // проверка координат мыши, не даёт вылезти курсору за пределы нашего слайдера блин :)
//        int posX = control_.pos_.x();
//        if (posX > maxWidth)
//            posX = maxWidth;
//        if (posX < 0)
//            posX = 0;
//        // перемещаем под мышку, рисуем
//        control_.cursorRect_.moveCenter(QPoint(posX,height()-helper_cursop_position));
//        painter.drawRect(control_.cursorRect_);
//    }
//    // восстанавливаем настройки паинтера
//    painter.restore();
//}

// нажатие мыши
void TimeLine::mousePressEvent ( QMouseEvent * pe )
{
    // если попадает на наш курсор, ставим флаг перемещение, иначе снимаем.
//    if (control_.cursorRect_.contains(pe->pos()))
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
    // передаём событие дальше(на всякий случай, если под слайдером ещё что-то есть)
    QWidget::mousePressEvent(pe);
}

// движение мыши.
void TimeLine::mouseMoveEvent(QMouseEvent * pe)
{
    // если флаг на движение стоит(попали мышкой в курсор и тянем)
    if (control_.moveDay)
    {
        // вычисляем куда его тянет, меняем значение переменной
        qDebug() << control_.pos_;
        int dx = control_.pos_.x() - pe->pos().x();
        // 2 minutes in pixel + 60 seconds in minute
        control_.currentDate = (control_.currentDate.addSecs(2*60*dx));
//        if (dx > 0 && )
//            control_.currentDate.setDate(control_.currentDate.addDays());
        qDebug() << control_.currentDate.time();
//        checkMouse();
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
//    repaint();
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
//    painter.save();
    painter.drawRect(0, 0, width(), height() - 24);

//    painter.end();

//    painter.begin(this);
    pen.setColor(Qt::black);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
//    painter.save();

    int curDayPixel = control_.currentDate.time().hour() * 30 + control_.currentDate.time().minute() / 2;
//    qDebug() << "curDayPixel = " << curDayPixel;

    int prevDayPixel = curDayPixel + 720;

    int nextDayPixel = 720 - curDayPixel;
//    qDebug() << "pixelValue2 = " << nextDayPixel;

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

//    painter.end();
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
