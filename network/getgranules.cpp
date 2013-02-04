#include "getgranules.h"

GetGranules::GetGranules(QObject *parent) :
    QObject(parent)
{
    countGranule = 0;
    connect(this, SIGNAL(selfRun()), this, SLOT(run()));
}

// create Product structure from xml (from QDomElement)
Granule createGranuleFromXml(QDomElement domElement)
{
    Granule newGranule;

    // Get main fields
    newGranule.granuleName = domElement.firstChildElement("GranuleName").text();
    newGranule.granuleId = domElement.firstChildElement("Id").text().toInt();
    newGranule.productName = domElement.firstChildElement("Product").text();
    newGranule.productId = domElement.firstChildElement("ProductId").text().toInt();

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
        getGranulesForNewProduct();
    else
        getNewGranules();
}

void GetGranules::getGranulesForNewProduct()
{
    QNetworkAccessManager* networkManager = new QNetworkAccessManager();

    QNetworkReply* reply = networkManager->get(_request);
//    networkManager->deleteLater();
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyReadGranules()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(getErrorGranules(QNetworkReply::NetworkError)));
}

void GetGranules::getErrorGranules(QNetworkReply::NetworkError)
{
    qDebug() << "getErrorGranules";
}

void GetGranules::slotReadyReadGranules()
{
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
//                qDebug() << "==========================";
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

                QDomElement  mElement = mDocument.documentElement().firstChildElement("Granule");
                while ( !mElement.isNull() )
                {
                    Granule newGranule = createGranuleFromXml(mElement);
                    countGranule++;
//                    if (newGranule.startDate.date() == QDate(2013, 01, 7))
//                        qDebug() << newGranule.startDate;

                    if (!granulesHash->contains(QString::number(newGranule.granuleId)))
                    {
                        lock.lockForWrite();
                        granulesHash->insert(QString::number(newGranule.granuleId), newGranule);
                        lock.unlock();
                    }

                    mElement = mElement.nextSiblingElement("Granule");
                }
            }
//            qDebug() << "countGranule" << countGranule;
            if (!(countGranule % 100) && countGranule)
            {
                if (_request.url().toString().indexOf("skip") == -1)
                {
                    _request.setUrl(QUrl(_request.url().toString() + "&$skip=100"));
                }
                else
                {
                    _request.setUrl(QUrl(_request.url().toString().replace \
                                             ("skip="+QString::number(countGranule-100),
                                              "skip="+QString::number(countGranule))));
                }
                emit selfRun();
                return;
            }
        }
        emit timeLineRepaint();
    }
    reply->deleteLater();
}

void GetGranules::getNewGranules()
{
    QNetworkAccessManager* networkManager = new QNetworkAccessManager ();
//    networkManager->deleteLater();
    QNetworkReply* reply = networkManager->get(_request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyReadGranules()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(getErrorGranules(QNetworkReply::NetworkError)));
}


void GetGranules::getErrorNewGranules(QNetworkReply::NetworkError)
{
    qDebug() << "getErrorNewGranules";
}

// http://staging.satin.rshu.ru/Download.ashx?granule=<granule_id>&method=[ftp|opendap|image|kml]