#ifndef TILEDOWNLOADER_H
#define TILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QThread>
#include <QFileInfo>
#include <QStringList>
#include <QDebug>

struct Sleeper : private QThread
{
    static void msleep(unsigned long msecs) { QThread::msleep(msecs); }
};

class TileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit TileDownloader(qint32 separationValue, qint32 lonTileNumValue, qint32 latTileNumValue,
                            qint32 curZoomValue, QString texStorePath, QObject *parent = 0);

    virtual ~TileDownloader();
    QByteArray downloadedData() const;

signals:
    void resultReady(qint32 curZoom, qint32 lonTileNum, qint32 latTileNum);

private slots:

    void fileDownloaded(QNetworkReply* pReply);

private:
    qint32 curZoom;
    qint32 separation;
    qint32 latTileNum;
    qint32 lonTileNum;

    QUrl                  texUrl;
    QString               textureStorePath;
    QNetworkAccessManager m_WebCtrl;
    QByteArray            m_DownloadedData;
};

#endif // TILEDOWNLOADER_H
