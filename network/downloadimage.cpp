#include "downloadimage.h"

DownloadImage::DownloadImage(QObject *parent) :
    QThread(parent)
{
}

void DownloadImage::run()
{
    QNetworkAccessManager* m_netwManager = new QNetworkAccessManager(this);

    connect(m_netwManager, &QNetworkAccessManager::finished, this, &DownloadImage::slotReadyRead);

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
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Error in" << reply->url() << ":" << reply->errorString();
        return;
    }

    QByteArray jpegData = reply->readAll();
    QPixmap* pixmap = new QPixmap();
    pixmap->loadFromData(jpegData);
    pixmap->save(_imagePath);
    delete pixmap;
    emit downloaded(_imagePath);
    deleteLater();
}

void DownloadImage::getError(QNetworkReply::NetworkError)
{
    qWarning() << "Unable to download the image";
}
