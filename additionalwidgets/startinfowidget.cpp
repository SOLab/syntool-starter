#include "startinfowidget.h"

StartInfoWidget::StartInfoWidget(ConfigData *configData, QWidget *mainWindow, QWidget *parent):
    QDialog(parent)
{
    // for clear memory after closed this window
//    setAttribute(Qt::WA_ShowModal);
//    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose);

    _configData = configData;

    wgt = new QWidget(this);

    mainLayout = new QVBoxLayout(this);
    gridLayout = new QGridLayout(wgt);
//    gridLayout->setAlignment(Qt::AlignTop | Qt::AlignRight);

    label = new QLabel(tr("Syntool Starter developed in the Satellite Oceanography Laboratory of the "
                          "Russian State Hydrometeorological University (SOLab RSHU).") + "\n\n" +
                       tr("To get started, select a product by clicking \"Add product\" on the top menu."), wgt);
    label->setWordWrap(true);

    closeButton = new QPushButton("Close", wgt);
//    button->setFixedSize(48,48);
    connect(closeButton, &QPushButton::clicked, this, &StartInfoWidget::close);

    hideCheckBox = new QCheckBox(tr("Hide during next launch"), this);

    setFixedSize(512,224);

    gridLayout->addWidget(hideCheckBox, 1,0);
    gridLayout->addWidget(label, 0, 1);
    gridLayout->addWidget(closeButton, 1, 1, 1, 1, Qt::AlignRight);

    // add logo
    QImageReader *ir = new QImageReader;
    if (_configData->lang.toLower().startsWith("ru"))
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

void StartInfoWidget::closeEvent(QCloseEvent *ev)
{
    if (hideCheckBox->isChecked())
    {
        QSettings *settings = new QSettings(_configData->configFile, QSettings::IniFormat);
        settings->setValue("other/hide_start_widget", 1);
        settings->sync();
        delete settings;
    }
    QDialog::closeEvent(ev);
}
