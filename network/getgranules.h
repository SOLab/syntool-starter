#ifndef GETGRANULES_H
#define GETGRANULES_H

#include <QObject>
#include <QThread>
#include <QReadWriteLock>

#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QtXml/QDomDocument>

#include <more/ProductStructures.h>

#include <QDebug>

class GetGranules : public QObject
{
    Q_OBJECT
public:
    explicit GetGranules(QObject *parent = 0);
    void setSelectedProducts(QHash<QString, selectedProduct>* _selectedProducts,
                             QHash<QString, Granule> *_granulesHash);
    void setParameters(QNetworkRequest request, QString methodName);

protected:
    QHash<QString, selectedProduct>* selectedProducts;
    QHash<QString, Granule>* granulesHash;

    QNetworkAccessManager *networkManager;
    QByteArray currentRequest;

    QNetworkRequest _request;
    QString _methodName;
    int countGranule;

private:
    QReadWriteLock lock;

signals:
  void timeLineRepaint();
  void selfRun();
  void selfClose();
    
public slots:
    void run();
    void getNewGranules();
    void getGranulesForNewProduct();

    void getErrorGranules(QNetworkReply::NetworkError);
    void slotReadyReadGranules();

    void getErrorNewGranules(QNetworkReply::NetworkError);

};

#endif // GETGRANULES_H
