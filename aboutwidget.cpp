#include "aboutwidget.h"

AboutWidget::AboutWidget(QWidget *parent) :
    QWidget(parent)
{
    QString copyright = "&copy;";
    QString curYear = QString::number(QDate::currentDate().year());

    QString companyName = "SOLab";
    QString companySite = "<a href='http://solab.rshu.ru/'>http://solab.rshu.ru/</a>";

    QString helpText = "SynTool 3D standalone application. " + QString("<br>%1 %2 2012 - %3<br>%4").arg(companyName).arg(copyright)
            .arg(curYear).arg(companySite);

    QString htmlHelp = "<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.0//EN' "
              "'http://www.w3.org/TR/REC-html40/strict.dtd'>"
              "<html><head><style type=text/css>"
              "</style></head><body>" + helpText + "</body></html>";

    statusLabel = new QLabel(htmlHelp);
    statusLabel->setOpenExternalLinks(true);

    QImageReader *ir = new QImageReader(":/logo-en.png");
    imageLabel = new QLabel(this);

    QPixmap pm2 = QPixmap().fromImageReader(ir);

    imageLabel->setPixmap(pm2);

    topLayout = new QHBoxLayout(this);
    topLayout->addWidget(imageLabel);
    topLayout->addWidget(statusLabel);

    setWindowTitle(tr("About programm"));
    resize(250, 140);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowModality(Qt::ApplicationModal);

    setFocusPolicy(Qt::StrongFocus);
    hide();
    show();
}
