#ifndef GETGRANULES_H
#define GETGRANULES_H

#include <QObject>
#include <QThread>
#include <QReadWriteLock>

#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QtXml/QDomDocument>

#include "more/ProductStructures.h"

#include <QDebug>

class GetGranules : public QObject
{
    Q_OBJECT
public:
    explicit GetGranules(QObject *parent = 0);
    void setSelectedProducts(QHash<QString, selectedProduct>* selectedProductsValue,
                             QHash<QString, Granule> *granulesHashValue);
    void setParameters(QNetworkRequest request, QString methodName);

protected:
    QHash<QString, selectedProduct>* selectedProducts;
    QHash<QString, Granule>*         granulesHash;
    QNetworkAccessManager*           networkManager;
    QByteArray                       currentRequest;

    QNetworkRequest m_request;
    QString         m_methodName;
    int             countGranule;
    int             currentCountGranule;
    int             currentStep;
    bool            newProduct;

private:
    QReadWriteLock lock;

signals:
  void timeLineRepaint();
  void selfRun();
  void moveTimeLine(QDateTime);
//  void selfClose();
    
public slots:
    void run();
    void getNewGranules();
    void getGranulesForNewProduct();

    void getErrorGranules(QNetworkReply::NetworkError);
    void slotReadyReadGranules();

    void getErrorNewGranules(QNetworkReply::NetworkError);

};

#endif // GETGRANULES_H
