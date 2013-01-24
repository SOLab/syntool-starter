#include "productinfowidget.h"

ProductInfoWidget::ProductInfoWidget(QWidget *parent) :
    QWidget(parent)
{
    gridLayout = new QGridLayout(this);
}

void ProductInfoWidget::setProduct(struct Product product, QPixmap* productImagePixmap)
{
    Name = new QLabel(this);
    Name->setText("Product name: "+product.Name);
    setWindowTitle("Information about "+product.Name);

    NaiadProductId = new QLabel(this);
    NaiadProductId->setText("Naiad product Id: "+product.NaiadProductId);
    StartDate = new QLabel(this);
    StartDate->setText("Start date: " + product.StartDate.toString("dd.MM.yyyy"));
    EndDate = new QLabel(this);
    EndDate->setText("End date: " + product.EndDate.toString("dd.MM.yyyy"));
    Description = new QLabel(this);
    Description->setText(product.Description);
    Description->setWordWrap(true);

    gridLayout->addWidget(Name,0,0);
    gridLayout->addWidget(NaiadProductId,1,0);
    gridLayout->addWidget(StartDate,2,0);
    gridLayout->addWidget(EndDate,3,0);

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
    gridLayout->addWidget(productParameters,4,0);

    // add more values
    int i = 5;
    if (!product.ProcessingLevels.isEmpty())    {
        QLabel* ProcessingLevels = new QLabel(this);
        ProcessingLevels->setText("Processing levels: "+product.ProcessingLevels);
        gridLayout->addWidget(ProcessingLevels,i,0);
        i++;
    }

    if (product.TotalGranulesCount != 0)    {
        QLabel* TotalGranulesCount = new QLabel(this);
        TotalGranulesCount->setText("Total granules count: " + \
                                    QString::number(product.TotalGranulesCount));
        gridLayout->addWidget(TotalGranulesCount,i,0);
        i++;
    }

    if (!product.ProductionInterval.isEmpty())    {
        QLabel* ProductionInterval = new QLabel(this);
        ProductionInterval->setText("Production interval: "+product.ProductionInterval);
        gridLayout->addWidget(ProductionInterval,i,0);
        i++;
    }


    gridLayout->addWidget(Description,i,0,1,2);

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
