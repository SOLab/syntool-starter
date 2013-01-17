#include "productswidget.h"

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
    comboProducts->setMaximumWidth(100);
    connect(comboProducts, SIGNAL(currentIndexChanged(int)), this, SLOT(currentProductChanged(int)));

    vLayout->addWidget(comboProducts);

    QLabel* AreaLbl = new QLabel("Select Area:", this);
    AreaLbl->setContentsMargins(4,4,0,0);
    vLayout->addWidget(AreaLbl);

    North = new InputBox("North: ", this);
    North->setValidator("double");
    North->setDisabled(true);

    South = new InputBox("South: ", this);
    South->setValidator("double");
    South->setDisabled(true);

    West = new InputBox("West: ", this);
    West->setValidator("double");
    West->setDisabled(true);

    East = new InputBox("East: ", this);
    East->setValidator("double");
    East->setDisabled(true);

    vLayout->addWidget(North);
    vLayout->addWidget(South);
    vLayout->addWidget(West);
    vLayout->addWidget(East);

    setObjectName("ProductsWidget");
    setStyleSheet(QString("QWidget#ProductsWidget {background-color: "
                  "rgb(%1, %2, %3);}")
                  .arg(parent->palette().background().color().red())
                  .arg(parent->palette().background().color().green())
                  .arg(parent->palette().background().color().blue()));
}

void ProductsWidget::currentProductChanged(int index)
{
    if (index == 0)
    {
        North->setDisabled(true);
        South->setDisabled(true);
        West->setDisabled(true);
        East->setDisabled(true);
    }
    else
    {
        North->setEnabled(true);
        South->setEnabled(true);
        West->setEnabled(true);
        East->setEnabled(true);
    }
}
