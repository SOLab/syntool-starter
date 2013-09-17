#include "startinfowidget.h"

StartInfoWidget::StartInfoWidget(ConfigData *configData, QWidget *mainWindow, QWidget *parent):
    QDialog(parent)
{
    // for clear memory after closed this window
//    setAttribute(Qt::WA_ShowModal);
//    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);

    wgt = new QWidget(this);

    mainLayout = new QVBoxLayout(this);
    gridLayout = new QGridLayout(wgt);
//    gridLayout->setAlignment(Qt::AlignTop | Qt::AlignRight);

    label = new QLabel(tr("Syntool Starter developed in the Satellite Oceanography Laboratory of the "
                          "Russian State Hydrometeorological University (SOLab RSHU).") + "\n\n" +
                       tr("To get started, select a product by clicking \"Add product\" on the top menu."), wgt);
    label->setWordWrap(true);

    button = new QPushButton("Close", wgt);
//    button->setFixedSize(48,48);

    connect(button, &QPushButton::clicked, this, &StartInfoWidget::close);

    setFixedSize(512,224);

    gridLayout->addWidget(label, 0, 1);
    gridLayout->addWidget(button, 1, 1);

    // add logo
    QImageReader *ir = new QImageReader;
    if (configData->lang.toLower().startsWith("ru"))
        ir->setFileName(":/logo-ru.png");
    else
        ir->setFileName(":/logo-en.png");
    imageLabel = new QLabel(this);

    QPixmap pm2 = QPixmap().fromImageReader(ir);
    delete ir;

    imageLabel->setPixmap(pm2);
    gridLayout->addWidget(imageLabel, 0, 0);

    wgt->setStyleSheet("QWidget{background-color:#EEEEEE;}");

    mainLayout->addWidget(wgt);
    mainLayout->setContentsMargins(0,0,0,0);
    setWindowTitle("SynTool Starter");

    move(mainWindow->width()/2 - size().width()/2,
         mainWindow->height()/2 - size().height()/2);
}
