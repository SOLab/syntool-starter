#include "aboutwidget.h"

AboutWidget::AboutWidget(QWidget *parent) :
    QWidget(parent)
{
    statusLabel = new QLabel(tr("AboutWidget text"));

    topLayout = new QHBoxLayout;
    topLayout->addWidget(statusLabel);
//    topLayout->addWidget();
    setLayout(topLayout);
    setWindowTitle(tr("About programm"));
    resize(250, 200);
    show();
}
