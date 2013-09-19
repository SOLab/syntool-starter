#include "aboutwidget.h"

AboutWidget::AboutWidget(ConfigData *configData, QWidget *mainWindow, QWidget *parent):
    QWidget()
{
    QString copyright = "&copy;";
    QString curYear = QString::number(QDate::currentDate().year());

    QString companyName = "SOLab";
    QString companySite = "<a href='http://solab.rshu.ru/'>http://solab.rshu.ru/</a>";

    QString helpText = tr("SynTool Starter 3D standalone application")+". " + QString("<br>%1 %2 2012 - %3<br>Website: %4")
            .arg(companyName).arg(copyright).arg(curYear).arg(companySite);

    QString htmlHelp = "<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.0//EN' "
              "'http://www.w3.org/TR/REC-html40/strict.dtd'>"
              "<html><head><style type=text/css>"
              "</style></head><body>" + helpText + "</body></html>";

    statusLabel = new QLabel(htmlHelp, this);
    statusLabel->setOpenExternalLinks(true);

    QImageReader *ir = new QImageReader;
    if (configData->lang.toLower().startsWith("ru"))
        ir->setFileName(":/logo-ru.png");
    else
        ir->setFileName(":/logo-en.png");
    imageLabel = new QLabel(this);

    QPixmap pm2 = QPixmap().fromImageReader(ir);
    delete ir;

    imageLabel->setPixmap(pm2);

    topLayout = new QHBoxLayout(this);
    topLayout->addWidget(imageLabel);
    topLayout->addWidget(statusLabel);

    setWindowTitle(tr("About program"));
    resize(250, 140);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowModality(Qt::ApplicationModal);

    setFocusPolicy(Qt::StrongFocus);
    hide();
    show();
    move(mainWindow->pos().x() + mainWindow->width()/2 - size().width()/2,
         mainWindow->pos().y() + mainWindow->height()/2 - size().height()/2);
}
