#include "productswidget.h"
#include <QSizePolicy>

ProductsWidget::ProductsWidget(QWidget *parent) :
    QWidget(parent)
{
    vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    productsLbl = new QLabel("Products list:", this);

    comboProducts = new QComboBox(this);
    comboProducts->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    connect(comboProducts, SIGNAL(currentIndexChanged(int)), this, SLOT(currentProductChanged(int)));

    reloadProductsButton = new QPushButton("Reload list", this);
    connect(reloadProductsButton, SIGNAL(clicked()), this, SLOT(reloadProductsList()));
    reloadProductsButton->hide();

    vLayout->addWidget(productsLbl);
    vLayout->addWidget(comboProducts);
    vLayout->addWidget(reloadProductsButton);

// Create request Url
    url = QUrl("http://staging.satin.rshu.ru/api/products");

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

void ProductsWidget::slotReadyRead()
{
    qDebug() << "++++++++++++++++++++++++++++++";
    reloadProductsButton->hide();

    QStringList productsList;
    productsList << "--not selected--";
    QNetworkReply *reply=qobject_cast<QNetworkReply*>(sender());
//    disconnect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
//    disconnect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
//               this, SLOT(getError(QNetworkReply::NetworkError)));
//    QNetworkAccessManager *manager=qobject_cast<QNetworkAccessManager *>(reply->manager());

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
                QString string(bytes);
//                qDebug() << string;

                QDomDocument mDocument;
                bool namespaceProcessing;
                QString errorMsg;
                int errorLine;
                int errorColumn;
                if (!mDocument.setContent(bytes, namespaceProcessing, &errorMsg, &errorLine, &errorColumn))
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
                    // do all you need to do with <name> element
                    QDomElement de = mElement.firstChildElement("NaiadProductId");
                    productsList << de.text();
                    mElement = mElement.nextSiblingElement("Product");
                }
            }
        }
    }

    comboProducts->clear();
    comboProducts->addItems(productsList);

//    disconnect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
//    disconnect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
//            this, SLOT(getError(QNetworkReply::NetworkError)));
//    reply->close();
//    delete reply;
}

void ProductsWidget::getError(QNetworkReply::NetworkError)
{
    reloadProductsButton->show();
    comboProducts->clear();
    comboProducts->addItem("Connection Error");
}

void ProductsWidget::reloadProductsList()
{
    qDebug() << url;

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
    North->setEnabled(enabledFlag);
    South->setEnabled(enabledFlag);
    West->setEnabled(enabledFlag);
    East->setEnabled(enabledFlag);
    productDateStart->setEnabled(enabledFlag);
    productTimeStart->setEnabled(enabledFlag);
    productDateEnd->setEnabled(enabledFlag);
    productTimeEnd->setEnabled(enabledFlag);
}
