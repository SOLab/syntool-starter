#ifndef GETGRANULECOORDS_H
#define GETGRANULECOORDS_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomElement>

#include <QDebug>

class GetGranuleCoords : public QObject
{
    Q_OBJECT
public:
    explicit GetGranuleCoords(QObject *parent = 0);
    void getCoords(QString serverName, qint32 granuleId);

protected:
    QString _serverName;
    int _granuleId;
    QNetworkAccessManager* networkManager;
    QNetworkRequest request;
    QByteArray currentRequest;

signals:
    void coordsSignal(qint32 granuleId, float north, float east, float south, float west);
    
public slots:
    void getErrorCoords(QNetworkReply::NetworkError);
    void slotReadyReadCoords();
    
};

#endif // GETGRANULECOORDS_H
