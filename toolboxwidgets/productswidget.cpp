#include "productswidget.h"
#include <QCalendarWidget>

ProductsWidget::ProductsWidget(QWidget *parent) :
    QWidget(parent)
{
    vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

// Add Product list

    QStringList productsList;
    productsList << "--not selected--"
                 << "ASCAT L2 12km"
                 << "ERSST-V3B"
                 << "OISST-AVHRR-AMSR-V2"
                 << "EUR-L2P-AVHRR_METOP_A"
                 << "IFR-L4-SSTfnd-ODYSSEA-GLOB_010"
                 << "IFR-L4-SSTfnd-ODYSSEA-MED_002"
                 << "OISST-AVHRR-V2";

    QLabel* productsLbl = new QLabel("Products list:", this);
    productsLbl->setContentsMargins(4,4,0,0);
    vLayout->addWidget(productsLbl);

    comboProducts = new QComboBox(this);
    comboProducts->addItems(productsList);
    comboProducts->setMaximumWidth(100);
    connect(comboProducts, SIGNAL(currentIndexChanged(int)), this, SLOT(currentProductChanged(int)));

    vLayout->addWidget(comboProducts);

// Add widgets for select Area

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

// Add widgets for temporal area
// From

    startProductDateValue = new QDate;

    productDateStart = new QDateEdit;
    productDateStart->setCalendarPopup(true);
    productDateStart->setDate(startProductDateValue->currentDate());
    productDateStart->setDisabled(true);
    vLayout->addWidget(productDateStart);

    startProductTimeValue = new QTime;

    productTimeStart = new QTimeEdit;
    productTimeStart->setTime(startProductTimeValue->currentTime());
    productTimeStart->setDisabled(true);
    vLayout->addWidget(productTimeStart);

// To

    endProductDateValue = new QDate;

    productDateEnd = new QDateEdit;
    productDateEnd->setCalendarPopup(true);
    productDateEnd->setDate(endProductDateValue->currentDate());
    productDateEnd->setDisabled(true);
    vLayout->addWidget(productDateEnd);

    endProductTimeValue = new QTime;

    productTimeEnd = new QTimeEdit;
    productTimeEnd->setTime(endProductTimeValue->currentTime());
    productTimeEnd->setDisabled(true);
    vLayout->addWidget(productTimeEnd);

// Set style

    setObjectName("ProductsWidget");
    setStyleSheet(QString("QWidget#ProductsWidget {background-color: "
                  "rgb(%1, %2, %3);}")
                  .arg(parent->palette().background().color().red())
                  .arg(parent->palette().background().color().green())
                  .arg(parent->palette().background().color().blue()));
}

void ProductsWidget::currentProductChanged(int index)
{
    bool enabledFlag;
    if (index == 0)
    {
        enabledFlag = false;
    }
    else
    {
        enabledFlag = true;
    }
    North->setEnabled(enabledFlag);
    South->setEnabled(enabledFlag);
    West->setEnabled(enabledFlag);
    East->setEnabled(enabledFlag);
    productDateStart->setEnabled(enabledFlag);
    productTimeStart->setEnabled(enabledFlag);
    productDateEnd->setEnabled(enabledFlag);
    productTimeEnd->setEnabled(enabledFlag);
}
