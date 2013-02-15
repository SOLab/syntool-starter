#include "calendar.h"

Calendar::Calendar(QDate date, QWidget *parent) :
    QWidget(parent)
{
    gridLayout = new QGridLayout(this);

    okButton = new QPushButton("Set date", this);

    cancelButton = new QPushButton("Cancel", this);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    calendar = new QCalendarWidget(this);
    calendar->setSelectedDate(date);
    calendar->setGridVisible(true);
    calendar->setStyleSheet("QSpinBox {width: 50px;}");

    gridLayout->addWidget(calendar, 0,0,2,2);
    gridLayout->addWidget(okButton, 2,0);
    gridLayout->addWidget(cancelButton, 2,1);

    resize(sizeHint());
    setWindowModality(Qt::ApplicationModal);
}
