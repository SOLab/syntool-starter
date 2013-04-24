#include "getgranules.h"

GetGranules::GetGranules(QObject *parent) :
    QObject(parent)
{
    countGranule = 0;
    currentCountGranule = 0;
    currentStep = 0;
    networkManager = new QNetworkAccessManager(this);
    connect(this, &GetGranules::selfRun, this, &GetGranules::run);
//    connect(this, &GetGranules::selfClose, this, &GetGranules::deleteLater);
}

// create Product structure from xml (from QDomElement)
Granule createGranuleFromXml(QDomElement domElement)
{
    Granule newGranule;

    // Get main fields
    newGranule.granuleName = domElement.firstChildElement("GranuleName").text();
    newGranule.granuleId = domElement.firstChildElement("Id").text().toInt();
//    newGranule.productName = domElement.firstChildElement("Product").text();
    newGranule.productId = domElement.firstChildElement("ProductId").text().toInt();
    newGranule.status = domElement.firstChildElement("Status").text().toInt();

    // get start and end date
    newGranule.startDate = QDateTime::fromString(domElement.firstChildElement("ProducedAt").text(),
                                                 Qt::ISODate);
//    newGranule.endDate = QDateTime::fromString(domElement.firstChildElement("EndDate").text(),
//                                               Qt::ISODate);
    return newGranule;
}

void GetGranules::setSelectedProducts(QHash<QString, selectedProduct> *_selectedProducts,
                                      QHash<QString, Granule> *_granulesHash)
{
    selectedProducts = _selectedProducts;
    granulesHash = _granulesHash;
}

void GetGranules::setParameters(QNetworkRequest request, QString methodName)
{
    _request = request;
    _methodName = methodName;
}

void GetGranules::run()
{
    if (_methodName == "getGranulesForNewProduct")
        // get granules for new added products
        getGranulesForNewProduct();
    else
        // get new granules
        getNewGranules();
}

void GetGranules::getGranulesForNewProduct()
{
    QNetworkReply* reply = networkManager->get(_request);
    connect(reply, &QNetworkReply::readyRead, this, &GetGranules::slotReadyReadGranules);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(getErrorGranules(QNetworkReply::NetworkError)));
}

void GetGranules::getErrorGranules(QNetworkReply::NetworkError)
{
    qWarning() << "Error getting the granules";
}

void GetGranules::getNewGranules()
{
    QNetworkReply* reply = networkManager->get(_request);
    connect(reply, &QNetworkReply::readyRead, this, &GetGranules::slotReadyReadGranules);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(getErrorGranules(QNetworkReply::NetworkError)));
}

void GetGranules::getErrorNewGranules(QNetworkReply::NetworkError)
{
    qWarning() << "Error getting the new granules";
}

void GetGranules::slotReadyReadGranules()
{
    QNetworkReply *reply=qobject_cast<QNetworkReply*>(sender());

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    QString nextStepElement = "false";

    if (reply->error() == QNetworkReply::NoError)
    {
        qDebug() << "status code: " << statusCode;

        switch (statusCode)
        {
            case 200:
            {
                QByteArray bytes = reply->readAll();

                QDomDocument mDocument;
                QString errorMsg;
                int errorLine;
                int errorColumn;
                if (!mDocument.setContent(bytes, false, &errorMsg,
                                          &errorLine, &errorColumn))
                {
                    qWarning() << "Error parse XML";
//                    qDebug() << errorMsg;
//                    qDebug() << errorLine;
//                    qDebug() << errorColumn;
                    if (errorLine > 1)
                    {
                        currentRequest = bytes;
                        return;
                    }
                    else
                    {
                        currentRequest = currentRequest + bytes;
                        bytes = currentRequest;
                        if (!mDocument.setContent(bytes, false, &errorMsg,
                                                  &errorLine, &errorColumn))
                        {
                                qCritical() << "Error parse XML";
                                qCritical() << errorMsg;
                                qCritical() << errorLine;
                                qCritical() << errorColumn;
                                return;
                        }
                    }
                }

                currentRequest.clear();

                nextStepElement = mDocument.documentElement().firstChildElement("NextStep").text();

                QDomElement  mElement = mDocument.documentElement().firstChildElement("Granules").
                                                                    firstChildElement("Granule");
                while ( !mElement.isNull() )
                {
                    Granule newGranule = createGranuleFromXml(mElement);
                    countGranule++;

                    if (!granulesHash->contains(QString::number(newGranule.granuleId)))
                    {
                        lock.lockForWrite();
                        granulesHash->insert(QString::number(newGranule.granuleId), newGranule);
                        lock.unlock();
                    }

                    mElement = mElement.nextSiblingElement("Granule");
                }
            }

            if (nextStepElement == "true")
            {
                currentStep += 1;
//                currentCountGranule = 0;
                if (_request.url().toString().indexOf("step=") == -1)
                {
                    _request.setUrl(QUrl(_request.url().toString() + "&step="+QString::number(currentStep)));
                }
                else
                {
                    _request.setUrl(QUrl(_request.url().toString().replace \
                                             ("step="+QString::number(currentStep-1),
                                              "step="+QString::number(currentStep))));
                }
                emit selfRun();
                return;
            }
        }
    }
    emit timeLineRepaint();
    reply->deleteLater();
    deleteLater();
}
