#include "aboutwidget.h"
#include <QDebug>

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

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowModality(Qt::ApplicationModal);

    setFocusPolicy(Qt::StrongFocus);
    hide();
    show();
}
