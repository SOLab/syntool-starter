#include "getgranulecoords.h"

GetGranuleCoords::GetGranuleCoords(QObject *parent) :
    QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
}

void GetGranuleCoords::getCoords(QString serverName, qint32 granuleId)
{
    _serverName = serverName;
    _granuleId = granuleId;

    request = QNetworkRequest(QUrl(_serverName+"/api/granules/"+QString::number(_granuleId)+"/coordinates"));
    request.setRawHeader("Content-Type", "text/xml");

    request.setUrl(request.url());
    qDebug() << "Request: " << request.url();


    QNetworkReply* reply = networkManager->get(request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyReadCoords()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(getErrorCoords(QNetworkReply::NetworkError)));
}

void GetGranuleCoords::slotReadyReadCoords()
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
                QString string(bytes);
                qDebug() << "==========================";
                qDebug() << string;

                QDomDocument mDocument;
//                bool namespaceProcessing;
                QString errorMsg;
                int errorLine;
                int errorColumn;
                if (!mDocument.setContent(bytes, false, &errorMsg,
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
                        if (!mDocument.setContent(bytes, false, &errorMsg,
                                                  &errorLine, &errorColumn))
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

                QDomElement  coordsElement = mDocument.documentElement();//.firstChildElement("Coordinates");
//                while ( !coordsElement.isNull() )
//                {
                    // North
                    float north = coordsElement.firstChildElement("maxLat").text().toFloat();
                    // East
                    float east = coordsElement.firstChildElement("maxLon").text().toFloat();
                    // South
                    float south = coordsElement.firstChildElement("minLat").text().toFloat();
                    // West
                    float west = coordsElement.firstChildElement("minLon").text().toFloat();

                    emit coordsSignal(_granuleId, north, east, south, west);

//                    coordsElement = coordsElement.nextSiblingElement("Coordinates");
//                }
            }
        }
    }
}

void GetGranuleCoords::getErrorCoords(QNetworkReply::NetworkError)
{
    qDebug() << "getErrorCoords";
}
