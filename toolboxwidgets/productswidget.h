#ifndef PRODUCTSWIDGET_H
#define PRODUCTSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>

#include <QCalendarWidget>
#include <QDate>
#include <QTime>
#include <QDateEdit>
#include <QTimeEdit>
#include "additionalwidgets/inputbox.h"
#include "additionalwidgets/productinfowidget.h"

#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QDebug>
#include <QPushButton>
#include <QtXml/QDomDocument>

#include <QHash>
#include "network/downloadimage.h"
#include <QFile>
#include <QDir>

#include "more/ProductStructures.h"
#include "more/structure.h"

#include "timeline.h"
#include "network/getgranules.h"

class ProductsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProductsWidget(ConfigData *configData, QWidget *parent = 0);
    void setSelectedProducts(QHash<QString, selectedProduct>* _selectedProducts,
                             QHash<QString, Granule> *_granulesHash);
    void setObjectsPointer(TimeLine* _timeLinePointer);

protected:
    QVBoxLayout* vLayout;
    QDomDocument* dom;

    QString serverName;
    QString cacheDir;
    QUrl urlProducts;
    QUrl urlGranules;
    QNetworkAccessManager *networkManager;
//    QNetworkReply *reply;
    QByteArray currentRequest;

    QLabel* productsLbl;
    QPushButton* viewProductInfo;

    QComboBox* comboProducts;
    QLabel* productImageLbl;
    QPixmap* productImagePixmap;
    QPushButton* reloadProductsButton;

    InputBox* North;
    InputBox* South;
    InputBox* West;
    InputBox* East;

//    QDate* startProductDateValue;
//    QTime* startProductTimeValue;
//    QDateEdit* productDateStart;
//    QDateEdit* productDateEnd;

//    QDate* endProductDateValue;
//    QTime* endProductTimeValue;
//    QTimeEdit* productTimeStart;
//    QTimeEdit* productTimeEnd;

    QLabel* parametersLbl;
    QComboBox* comboParameters;

    QPushButton* addProductLabel;

    QHash<QString, int> parametersList;
    // <productNaiadId, Product>
    QHash<QString, Product>* productsHash;
    // <productId, productNaiadId>
    QHash<qint32, QString>* productsIdName;

    QHash<QString, selectedProduct>* selectedProducts;
    QHash<QString, Granule>* granulesHash;

    void getGranulesForNewProduct();
    TimeLine* timeLinePointer;
    GetGranules* getGranulesPointer;
signals:
    void productAdded(QString productId);
    void productDeleted(QString productId);
    void productsHashSignal(QHash<QString, Product>* productsHash, QHash<qint32, QString>* productsIdName);
    
public slots:
    void currentProductChanged(int index);
    void slotReadyReadProductList();
    void getErrorProductList(QNetworkReply::NetworkError);

    void getNewGranules(int scale);

    void reloadProductsList();
    void addProduct();
    void slotProductInfo();

    void removeProduct(QString productId);
};

#endif // PRODUCTSWIDGET_H
