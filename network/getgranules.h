#ifndef GETGRANULES_H
#define GETGRANULES_H

#include <QObject>

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

protected:
    QHash<QString, selectedProduct>* selectedProducts;
    QHash<QString, Granule>* granulesHash;

    QNetworkAccessManager *networkManager;
    QByteArray currentRequest;

signals:
  void timeLineRepaint();
    
public slots:
    void getNewGranules(QNetworkRequest request);
    void getGranulesForNewProduct(QNetworkRequest request);

    void getErrorGranules(QNetworkReply::NetworkError);
    void slotReadyReadGranules();

    void getErrorNewGranules(QNetworkReply::NetworkError);
    void slotReadyReadNewGranules();

};

#endif // GETGRANULES_H
