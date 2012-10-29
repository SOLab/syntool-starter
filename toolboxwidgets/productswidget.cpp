#include "productswidget.h"
#include <QDebug>

ProductsWidget::ProductsWidget(QWidget *parent) :
    QWidget(parent)
{
    comboProducts = new QComboBox(this);
    comboProducts->addItem("--not selected--");
    comboProducts->addItem("ASCAT L2 12km");
    comboProducts->addItem("ERSST-V3B");
    comboProducts->addItem("OISST-AVHRR-AMSR-V2");
    comboProducts->addItem("EUR-L2P-AVHRR_METOP_A");
    comboProducts->addItem("IFR-L4-SSTfnd-ODYSSEA-GLOB_010");
    comboProducts->addItem("IFR-L4-SSTfnd-ODYSSEA-MED_002");
    comboProducts->addItem("OISST-AVHRR-V2");

    vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->addWidget(comboProducts, 0, Qt::AlignTop);

    setObjectName("ProductsWidget");
    setStyleSheet(QString("QWidget#ProductsWidget {background-color: "
                  "rgb(%1, %2, %3);}")
                  .arg(parent->palette().background().color().red())
                  .arg(parent->palette().background().color().green())
                  .arg(parent->palette().background().color().blue()));
}
