#include "tiledownloader.h"

TileDownloader::TileDownloader(qint32 _separation, qint32 _lonTileNum, qint32 _latTileNum,
                               qint32 _cur_zoom, QString texStorePath, QObject *parent):
    QObject(parent)
{
    separation = _separation;
    lonTileNum = _lonTileNum;
    latTileNum = _latTileNum;
    cur_zoom = _cur_zoom;

    QString path = "http://tile.openstreetmap.org/%1/%2/%3.png";
    QString arg(path.arg(cur_zoom).arg(lonTileNum).arg(separation-1-latTileNum));

    texUrl = QUrl::fromEncoded(arg.toLocal8Bit());

    textureStorePath = texStorePath;

    connect(&m_WebCtrl, &QNetworkAccessManager::finished,
            this, &TileDownloader::fileDownloaded);

    QNetworkRequest request(texUrl);
    m_WebCtrl.get(request);
}

TileDownloader::~TileDownloader()
{

}

void TileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    m_DownloadedData = pReply->readAll();
    //emit a signal
    pReply->deleteLater();

    if (QFile::exists(textureStorePath))
    {
        return;
    }

    if ( pReply->error() == QNetworkReply::NoError )
    {
      QFile image( textureStorePath );
      image.open(QIODevice::WriteOnly);
      image.write(m_DownloadedData);
      image.close();
    }
    emit resultReady(cur_zoom, lonTileNum, latTileNum);
}

QByteArray TileDownloader::downloadedData() const
{
    return m_DownloadedData;
}
