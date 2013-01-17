#include "productswidget.h"
#include <QDebug>

ProductsWidget::ProductsWidget(QWidget *parent) :
    QWidget(parent)
{
    QStringList productsList;
    productsList << "--not selected--"
                 << "ASCAT L2 12km"
                 << "ERSST-V3B"
                 << "OISST-AVHRR-AMSR-V2"
                 << "EUR-L2P-AVHRR_METOP_A"
                 << "IFR-L4-SSTfnd-ODYSSEA-GLOB_010"
                 << "IFR-L4-SSTfnd-ODYSSEA-MED_002"
                 << "OISST-AVHRR-V2";


    vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    QLabel* productsLbl = new QLabel("Products list:", this);
    productsLbl->setContentsMargins(4,4,0,0);
    vLayout->addWidget(productsLbl);

    comboProducts = new QComboBox(this);
    comboProducts->addItems(productsList);
    vLayout->addWidget(comboProducts);

    setObjectName("ProductsWidget");
    setStyleSheet(QString("QWidget#ProductsWidget {background-color: "
                  "rgb(%1, %2, %3);}")
                  .arg(parent->palette().background().color().red())
                  .arg(parent->palette().background().color().green())
                  .arg(parent->palette().background().color().blue()));
}
