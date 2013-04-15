#include "tiledownloader.h"

TileDownloader::TileDownloader(QUrl imageUrl, QString imagePath, QObject *parent):
    QObject(parent)
{
    textureStorePath = imagePath;
    connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)),
                SLOT(fileDownloaded(QNetworkReply*)));

    QNetworkRequest request(imageUrl);
    m_WebCtrl.get(request);
}

TileDownloader::~TileDownloader()
{

}

void TileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    qDebug() << "===============++>>>";
    m_DownloadedData = pReply->readAll();
    //emit a signal
    pReply->deleteLater();

    if (QFile::exists(textureStorePath))
    {
        return;
    }

//    qDebug() << "filepath" << m_filename;
    if ( pReply->error() == QNetworkReply::NoError )
    {
      QFile image( textureStorePath );
      image.open(QIODevice::WriteOnly);
      image.write(m_DownloadedData);
      image.close();
    }
    emit resultReady(textureStorePath);
}

QByteArray TileDownloader::downloadedData() const
{
    return m_DownloadedData;
}
