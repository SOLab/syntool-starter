#include "calendar.h"

Calendar::Calendar(QWidget *parent) :
    QWidget(parent)
{
    gridLayout = new QGridLayout(this);

    okButton = new QPushButton("Set date", this);

    cancelButton = new QPushButton("Cancel", this);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    calendar = new QCalendarWidget(this);

    gridLayout->addWidget(calendar, 0,0,2,2);
    gridLayout->addWidget(okButton, 2,0);
    gridLayout->addWidget(cancelButton, 2,1);

    resize(sizeHint());
}
