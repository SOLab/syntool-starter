#include "productinfowidget.h"

ProductInfoWidget::ProductInfoWidget(QWidget *parent) :
    QWidget(parent)
{
    gridLayout = new QGridLayout(this);
}

void ProductInfoWidget::setProduct(struct Product product, QPixmap* productImagePixmap)
{
    name = new QLabel(this);
    name->setText(tr("Product name: ")+product.Name);
    setWindowTitle(tr("Information about ")+product.Name);

    naiadProductId = new QLabel(this);
    naiadProductId->setText(tr("Naiad product Id: ")+product.NaiadProductId);
    productId = new QLabel(this);
    productId->setText(tr("Naiad Id: ")+QString::number(product.Id));
    startDate = new QLabel(this);
    startDate->setText(tr("Start date: ") + product.StartDate.toString("dd.MM.yyyy"));
    endDate = new QLabel(this);
    endDate->setText(tr("End date: ") + product.EndDate.toString("dd.MM.yyyy"));
    description = new QLabel(this);
    description->setText(product.Description);
    description->setWordWrap(true);
    description->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);

    gridLayout->addWidget(name,0,0);
    gridLayout->addWidget(naiadProductId,1,0);
    gridLayout->addWidget(productId,2,0);
    gridLayout->addWidget(startDate,3,0);
    gridLayout->addWidget(endDate,4,0);

    // Add parameters
    QLabel* productParameters = new QLabel(this);
    QString parametersString;

    for (int i = 0; i < product.productsParameters.keys().size(); ++i)
    {
        if (!parametersString.isEmpty())
            parametersString += ", ";
        else
            parametersString += "Product parameters: ";
        parametersString += product.productsParameters.keys().at(i);
    }
    productParameters->setText(parametersString);
    productParameters->setWordWrap(true);
    gridLayout->addWidget(productParameters,5,0);

    // add more values
    int i = 6;
    if (!product.ProcessingLevels.isEmpty())    {
        QLabel* ProcessingLevels = new QLabel(this);
        ProcessingLevels->setText(tr("Processing levels: ")+product.ProcessingLevels);
        gridLayout->addWidget(ProcessingLevels,i,0);
        i++;
    }

    if (product.TotalGranulesCount != 0)    {
        QLabel* TotalGranulesCount = new QLabel(this);
        TotalGranulesCount->setText(tr("Total granules count: ") + \
                                    QString::number(product.TotalGranulesCount));
        gridLayout->addWidget(TotalGranulesCount,i,0);
        i++;
    }

    if (!product.ProductionInterval.isEmpty())    {
        QLabel* ProductionInterval = new QLabel(this);
        ProductionInterval->setText(tr("Production interval: ")+product.ProductionInterval);
        gridLayout->addWidget(ProductionInterval,i,0);
        i++;
    }


    gridLayout->addWidget(description,i,0,1,2);

    if (!product.ImageUrl.isEmpty())
    {
        QLabel* ImageUrl = new QLabel(this);
        ImageUrl->setPixmap(*productImagePixmap);
        gridLayout->addWidget(ImageUrl, 0,1,i,1);
    }
}

void ProductInfoWidget::closeEvent(QCloseEvent *event)
{
    deleteLater();
    event->accept();
}
