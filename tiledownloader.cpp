#include "tiledownloader.h"

TileDownloader::TileDownloader(qint32 separationValue, qint32 lonTileNumValue, qint32 latTileNumValue,
                               qint32 curZoomValue, QString texStorePath, QObject *parent):
    QObject(parent)
{
    separation = separationValue;
    lonTileNum = lonTileNumValue;
    latTileNum = latTileNumValue;
    curZoom = curZoomValue;

    QString path = "http://tile.openstreetmap.org/%1/%2/%3.png";
    QString arg(path.arg(curZoom).arg(lonTileNum).arg(separation-1-latTileNum));

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
    emit resultReady(curZoom, lonTileNum, latTileNum);
}

QByteArray TileDownloader::downloadedData() const
{
    return m_DownloadedData;
}
