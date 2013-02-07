#include "granuleinfowidget.h"

GranuleInfoWidget::GranuleInfoWidget(Granule granule, QWidget *parent) :
    QWidget(parent)
{
//    qDebug() << granule.granuleName;
//    qDebug() << granule.granuleId;
//    qDebug() << granule.productName;
//    qDebug() << granule.productId;
//    qDebug() << granule.startDate;
//    qDebug() << granule.lat;
//    qDebug() << granule.lon;
    gridLayout = new QVBoxLayout(this);

    QLabel* granuleName = new QLabel(this);
    granuleName->setText(tr("Name: ")+granule.granuleName);

    QLabel* granuleId = new QLabel(this);
    granuleId->setText(tr("Id: ")+QString::number(granule.granuleId));

    QLabel* productId = new QLabel(this);
    productId->setText(tr("Product Id: ")+QString::number(granule.productId));

    QLabel* startDate = new QLabel(this);
    startDate->setText(tr("Start Date: ")+granule.startDate.toString("dd.MM.yyyy hh:mm:ss"));

    gridLayout->addWidget(granuleName);
    gridLayout->addWidget(granuleId);

    if (!granule.productName.isEmpty())
    {
        QLabel* productName = new QLabel(this);
        productName->setText(tr("Product name: ")+granule.productName);
        gridLayout->addWidget(productName);
    }

    gridLayout->addWidget(productId);
    gridLayout->addWidget(startDate);

    setFixedSize(sizeHint());
    setWindowIcon(QIcon(":/icons/properties.png"));
    setWindowTitle("Properties "+granule.granuleName);

//    show();
//    deleteLater();
//    qDebug() << granule.granuleId;
//    qDebug() << granule.productId;
//    qDebug() << granule.productName;
//    qDebug() << granule.startDate;
//    qDebug() << granule.lat;
//    qDebug() << granule.lon;
}
