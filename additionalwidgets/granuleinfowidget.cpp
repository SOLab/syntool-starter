#include "granuleinfowidget.h"

GranuleInfoWidget::GranuleInfoWidget(Granule granule, QWidget *parent) :
    QWidget(parent)
{
    vLayout = new QVBoxLayout(this);

    QLabel* granuleName = new QLabel(this);
    granuleName->setText(tr("Name: ")+granule.granuleName);

    QLabel* granuleId = new QLabel(this);
    granuleId->setText(tr("Id: ")+QString::number(granule.granuleId));

    QLabel* granuleStatus = new QLabel(this);
    granuleStatus->setText(tr("Status: ")+QString::number(granule.status));

    QLabel* productId = new QLabel(this);
    productId->setText(tr("Product Id: ")+QString::number(granule.productId));

    QLabel* startDate = new QLabel(this);
    startDate->setText(tr("Start Date: ")+granule.startDate.toString("dd.MM.yyyy hh:mm:ss"));

    vLayout->addWidget(granuleName);
    vLayout->addWidget(granuleId);

    vLayout->addWidget(granuleStatus);

//    if (!granule.productName.isEmpty())
//    {
//        QLabel* productName = new QLabel(this);
//        productName->setText(tr("Product name: ")+granule.productName);
//        gridLayout->addWidget(productName);
//    }

    vLayout->addWidget(productId);
    vLayout->addWidget(startDate);

    setFixedSize(sizeHint());
    setWindowIcon(QIcon(":/icons/properties.png"));
    setWindowTitle(tr("Properties")+" "+granule.granuleName);
}
