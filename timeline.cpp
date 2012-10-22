#include "timeline.h"

int partCount = 50;
int helper_cursop_position = 31;
#include <qdebug.h>

TimeLine::TimeLine(QWidget *parent)
: QWidget(parent)
{
    // минимальное, максимальное, текущее показываемое мин-макс, текущее значение (на каком отрезке стоит), размеры курсора, флаги движения и обновления диапазона
    control_.min_ = 0;
    control_.max_ = 200;
    control_.curMax_ = 50;
    control_.curMin_ = 0;
    control_.current_ = 20;
    QRect rect(0,0,10,18);
    control_.cursorRect_ = rect;
    control_.move_ = false;
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
    checkMouse();
}

void TimeLine::paintEvent(QPaintEvent * pe)
{
    Q_UNUSED(pe);
    QPainter painter(this);
    // рисуем значение текущее
    painter.drawText(40,10, QString::number(control_.current_ + control_.curMin_));
    // ручка чёрная, толщина 1, устанавливаем, сохраняем настройки.
    QPen pen(Qt::black);
    pen.setWidth(1);
    painter.setPen(pen);
    painter.save();
    // количество ячеек на линии, расчёт длины помещающихся отрезков,
    // целого числа(вот тут хз у меня чёто с формулой намудрено, но работает :D)
    int partCount = 50;
    int maxWidth = partCount * (width()/partCount);
    qDebug() << "==>" << maxWidth;
    qDebug() << width();
    maxWidth = width() - 1;

    // рисует рамку, рисуем основную линию, рисуем текст макс мин.
    painter.drawRect(0, height()-50, maxWidth, 40);
    painter.drawLine(0,height()-30,maxWidth,height()-30);
    painter.drawText(0, height()-60, QString::number(control_.curMin_));
    painter.drawText(maxWidth - 20, height()-60, QString::number(control_.curMax_));

    //  длина 1 отрезка, в цикле рисуем их дофига(50 штук, как и заказывали)
    int widthOnePart = (width() - 3)/ partCount;
    for (int i = 0; i <= partCount; i++)
    {
        painter.drawLine(widthOnePart*i, height()-40, widthOnePart*i, height()-20);
    }

    QBrush brush(Qt::red);
    painter.setBrush(brush);
//    painter.save();
    // проверка флага на движение мышкой
    if (!control_.move_)
    {
        // получаем координаты курсора - текущее значение на длину 1 отрезка.
        // Перемещаем наш курсор центром на это деление.
        // Рисуем :) Так же рисуем линию от центра до курсора( дебажная вещь, можно закомментить.)
        int tmpInt = (control_.current_) * widthOnePart;
        control_.cursorRect_.moveCenter(QPoint(tmpInt,height()-helper_cursop_position));
        painter.drawRect(control_.cursorRect_);
//		painter.drawLine(QPoint(width()/2,height()/2), control_.cursorRect_.center());
    }
    else
    {
        // проверка координат мыши, не даёт вылезти курсору за пределы нашего слайдера блин :)
        int posX = control_.pos_.x();
        if (posX > maxWidth)
            posX = maxWidth;
        if (posX < 0)
            posX = 0;
        // перемещаем под мышку, рисуем
        control_.cursorRect_.moveCenter(QPoint(posX,height()-helper_cursop_position));
        painter.drawRect(control_.cursorRect_);
    }
    // восстанавливаем настройки паинтера
    painter.restore();

}

// нажатие мыши
void TimeLine::mousePressEvent ( QMouseEvent * pe )
{
    // если попадает на наш курсор, ставим флаг перемещение, иначе снимаем.
    if (control_.cursorRect_.contains(pe->pos()))
        control_.move_ = true;
    else
        control_.move_ = false;
    // передаём событие дальше(на всякий случай, если под слайдером ещё что-то есть)
//    QWidget::mousePressEvent(pe);
}

// движение мыши.
void TimeLine::mouseMoveEvent(QMouseEvent * pe)
{
    // если флаг на движение стоит(попали мышкой в курсор и тянем)
    if (control_.move_)
    {
        // вычисляем куда его тянет, меняем значение переменной
        control_.pos_ = pe->pos();
        // вычисляем длину одного отрезка
        int x = width()/partCount;// 50 в данном случае количество отрезков - по идее надо переменную глобальную сделать, но время... int partCount = 50
        // эммм... сакральная строчка :D :D :D  расчёт значения где наш курсорище. И далее меняем переменную
        int cur = control_.cursorRect_.center().x()/x;
        control_.current_ = cur;
        // ф-ция как раз сдвига диапазона + перерисовка + отправка сообщеньки дальше
        checkMouse();
        repaint();
    }
    QWidget::mouseMoveEvent(pe);
}
void TimeLine::checkMouse()
{
    // если флаг на изменение диапазона тру, то меняем. Если нет, то вылетаем(таймер в начале гарантирует вызов раз в N секунд)
    if (control_.flagMovedIntervalos_)
    {
        // если максимум, сдвигаем на 25
        if (control_.current_ == 50)
        {
            if (control_.curMax_ != control_.max_)
            {
                control_.curMin_ +=25;
                control_.curMax_ +=25;
                control_.flagMovedIntervalos_ = false;
                backToMouseAndCursor(false);
            }
        }
        // если минимум, сдвигаем на -25
        if (control_.current_ == 0)
        {
            if (control_.curMin_ != control_.min_)
            {
                control_.curMin_ -=25;
                control_.curMax_ -=25;
                control_.flagMovedIntervalos_ = false;
                backToMouseAndCursor(true);
            }
        }

    }
    // перерисовываем
    repaint();
}

// отжатие - снимаем флаг.
void TimeLine::mouseReleaseEvent ( QMouseEvent * pe )
{
    control_.move_ = false;
    repaint();
    QWidget::mouseReleaseEvent (pe);
}

// отскакиваем мышкой и курсором от стенок сплиттера :)
void TimeLine::backToMouseAndCursor(bool inDirection)
{
    int tmpInt = 25;
    if (!inDirection)
        tmpInt = -25;
    control_.current_ +=tmpInt;
    control_.move_ = false;
    repaint();
    control_.move_ = true;
    QCursor::setPos(mapToGlobal(control_.cursorRect_.center()));
}
