#include "downloadimage.h"

DownloadImage::DownloadImage(QObject *parent) :
    QThread(parent)
{
}

void DownloadImage::run()
{
    QNetworkAccessManager* m_netwManager = new QNetworkAccessManager(this);

    connect(m_netwManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slotReadyRead(QNetworkReply*)));

    QNetworkRequest request(_imageUrl);
    QNetworkReply* reply = m_netwManager->get(request);

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(getError(QNetworkReply::NetworkError)));
}

void DownloadImage::setImageUrl(QString imageUrl, QString imagePath)
{
    _imageUrl = imageUrl;
    _imagePath = imagePath;
}

void DownloadImage::slotReadyRead(QNetworkReply *reply)
{
    qDebug() << "DownloadImage::slotReadyRead";
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Error in" << reply->url() << ":" << reply->errorString();
        return;
    }

    QByteArray jpegData = reply->readAll();
    QPixmap pixmap;
    pixmap.loadFromData(jpegData);
    pixmap.save(_imagePath);
    qDebug() << _imagePath;
//    label->setPixmap(pixmap); // or whatever your labels name is
    deleteLater();
}

void DownloadImage::getError(QNetworkReply::NetworkError)
{
    qDebug() << "DownloadImage::getError";
}
