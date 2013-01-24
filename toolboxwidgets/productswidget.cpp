#include "productswidget.h"
#include <QSizePolicy>

ProductsWidget::ProductsWidget(QWidget *parent) :
    QWidget(parent)
{
    vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0,2,0,0);
//    vLayout->setSpacing(3);
    vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    productsLbl = new QLabel("Products list:", this);

    comboProducts = new QComboBox(this);
    comboProducts->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    connect(comboProducts, SIGNAL(currentIndexChanged(int)), this, SLOT(currentProductChanged(int)));

    productImageLbl = new QLabel(this);
    productImageLbl->hide();
    productImagePixmap = new QPixmap;

    reloadProductsButton = new QPushButton("Reload list", this);
    connect(reloadProductsButton, SIGNAL(clicked()), this, SLOT(reloadProductsList()));
    reloadProductsButton->hide();

    vLayout->addWidget(productsLbl);
    vLayout->addWidget(comboProducts);
    vLayout->addWidget(productImageLbl);
    vLayout->addWidget(reloadProductsButton);

// Create request Url
    serverName = "http://staging.satin.rshu.ru";
    url = QUrl(serverName + "/api/products");

    reloadProductsList();

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

    QLabel* fromDate = new QLabel("From: ", this);
    vLayout->addWidget(fromDate);

    startProductDateValue = new QDate;

    productDateStart = new QDateEdit;
    productDateStart->setCalendarPopup(true);
    productDateStart->setDate(startProductDateValue->currentDate());
    productDateStart->setDisabled(true);
    productDateStart->setContentsMargins(2,0,2,0);
    vLayout->addWidget(productDateStart);

    startProductTimeValue = new QTime;

    productTimeStart = new QTimeEdit;
    productTimeStart->setTime(startProductTimeValue->currentTime());
    productTimeStart->setDisabled(true);
    productTimeStart->setContentsMargins(2,0,2,0);
    vLayout->addWidget(productTimeStart);

// To

    QLabel* toDate = new QLabel("To: ", this);
    vLayout->addWidget(toDate);

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

// select parameters

    parametersLbl = new QLabel("Select parameter: ", this);
    comboParameters = new QComboBox(this);
    comboParameters->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    comboParameters->setDisabled(true);

    vLayout->addWidget(parametersLbl);
    vLayout->addWidget(comboParameters);

// Set style

    setObjectName("ProductsWidget");
    setStyleSheet(QString("QWidget#ProductsWidget {background-color: "
                  "rgb(%1, %2, %3);}")
                  .arg(parent->palette().background().color().red())
                  .arg(parent->palette().background().color().green())
                  .arg(parent->palette().background().color().blue()));
}

// create Product structure from xml (from QDomElement)
Product createProductFromXml(QDomElement domElement)
{
    Product newProduct;

    // Get main fields
    newProduct.Name = domElement.firstChildElement("Name").text();
    newProduct.NaiadProductId = domElement.firstChildElement("NaiadProductId").text();
    newProduct.Description = domElement.firstChildElement("Description").text();
    newProduct.ProductionInterval = domElement.firstChildElement("ProductionInterval").text();
    newProduct.ImageUrl = domElement.firstChildElement("ImageUrl").text();

    // get start and end date
    newProduct.StartDate = QDateTime::fromString(domElement.firstChildElement("StartDate").text(),
                                                 Qt::ISODate);
    newProduct.EndDate = QDateTime::fromString(domElement.firstChildElement("EndDate").text(),
                                               Qt::ISODate);

    // Get parameters
    QDomElement productParameters = domElement.firstChildElement("Parameters");
    QDomElement productParameter = productParameters.firstChildElement("Parameter");

    while ( !productParameter.isNull() )
    {
        newProduct.productsParameters[productParameter.firstChildElement("Name").text()] = \
                                      productParameter.firstChildElement("Id").text().toInt();
        productParameter = productParameter.nextSiblingElement("Parameter");
    }

    return newProduct;
}

void ProductsWidget::slotReadyRead()
{
    reloadProductsButton->hide();

    QStringList productsList;
    productsList << "--not selected--";
    QNetworkReply *reply=qobject_cast<QNetworkReply*>(sender());

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qDebug() << "=================================";
    if (reply->error() == QNetworkReply::NoError)
    {
        qDebug() << "status code: " << statusCode;

        switch (statusCode)
        {
            case 200:
            {
                QByteArray bytes = reply->readAll();
//                QString string(bytes);
//                qDebug() << string;

                QDomDocument mDocument;
                bool namespaceProcessing;
                QString errorMsg;
                int errorLine;
                int errorColumn;
                if (!mDocument.setContent(bytes, namespaceProcessing, &errorMsg,
                                          &errorLine, &errorColumn))
                {
                    qDebug() << "Error XML";
                    qDebug() << errorMsg;
                    qDebug() << errorLine;
                    qDebug() << errorColumn;
                    if (errorLine > 1)
                    {
                        currentRequest = bytes;
                        return;
                    }
                    else
                    {
                        currentRequest = currentRequest + bytes;
                        bytes = currentRequest;
                        if (!mDocument.setContent(bytes, namespaceProcessing,
                                                  &errorMsg, &errorLine, &errorColumn))
                        {
                                qDebug() << "Error XML";
                                qDebug() << errorMsg;
                                qDebug() << errorLine;
                                qDebug() << errorColumn;
                                return;
                        }
                    }
                }
                currentRequest.clear();

                QDomElement  mElement = mDocument.documentElement().firstChildElement("Product");
                while ( !mElement.isNull() )
                {
                    Product newProduct = createProductFromXml(mElement);

                    QString imagePath = QString("/tmp/%1.jpg").arg(newProduct.NaiadProductId);

                    // download product image
                    if (!newProduct.ImageUrl.isEmpty())
                    {
                        if ( ! QFile::exists(imagePath) ) {
                            DownloadImage* downloadImage = new DownloadImage;
                            downloadImage->setImageUrl(serverName+newProduct.ImageUrl,
                                                       imagePath);
                            downloadImage->run();
                        }
                    }

                    QDomElement productID = mElement.firstChildElement("NaiadProductId");
                    productsHash[productID.text()] = newProduct;

                    // create parametersList <parameterName, parameterId>
                    QHash<QString, int>::const_iterator k = newProduct.productsParameters.constBegin();
                    while ( k != newProduct.productsParameters.constEnd() )
                    {
                        if (!parametersList.contains(k.key()))
                            parametersList[k.key()] = k.value();
                        ++k;
                    }

                    productsList << productID.text();

                    mElement = mElement.nextSiblingElement("Product");
                }
            }
        }
    }

    comboProducts->clear();
    comboProducts->addItems(productsList);
}

void ProductsWidget::getError(QNetworkReply::NetworkError)
{
    reloadProductsButton->show();
    comboProducts->clear();
    comboProducts->addItem("Connection Error");
}

void ProductsWidget::reloadProductsList()
{
//    qDebug() << url;

    QNetworkRequest request;
    request.setUrl(url);
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("Accept-Language", "ru,en-us;q=0.7,en;q=0.3");
    request.setRawHeader("Accept-Charset", "utf-8;q=0.7,*;q=0.7");
    request.setRawHeader("Content-Type", "text/xml");

    networkManager = new QNetworkAccessManager (this);
    QNetworkReply* reply = networkManager->get(request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(getError(QNetworkReply::NetworkError)));
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

    // if product not selected -> set disabled all elements
    if (enabledFlag != North->isEnabled())
    {
        North->setEnabled(enabledFlag);
        South->setEnabled(enabledFlag);
        West->setEnabled(enabledFlag);
        East->setEnabled(enabledFlag);
        productDateStart->setEnabled(enabledFlag);
        productTimeStart->setEnabled(enabledFlag);
        productDateEnd->setEnabled(enabledFlag);
        productTimeEnd->setEnabled(enabledFlag);
        comboParameters->setEnabled(enabledFlag);
    }

    comboParameters->clear();
    if (enabledFlag)
    {
        // add new parameters in comboParameters
        comboParameters->addItems( \
                    productsHash[comboProducts->currentText()].productsParameters.keys());
        comboParameters->model()->sort(0);

        // set product image to productImageLbl
        if (QFile::exists(QString("/tmp/%1.jpg").\
                          arg(productsHash[comboProducts->currentText()].NaiadProductId)))
        {
            productImagePixmap->load(QString("/tmp/%1.jpg").\
                                 arg(productsHash[comboProducts->currentText()].NaiadProductId));
            *productImagePixmap = productImagePixmap->scaled(116,116);

            productImageLbl->setPixmap(*productImagePixmap);
            productImageLbl->show();
        }
        else {
            productImageLbl->hide();
        }
    }
    else
    {
        productImageLbl->hide();
    }
}
