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
#include <additionalwidgets/inputbox.h>
#include <additionalwidgets/productinfowidget.h>

#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QDebug>
#include <QPushButton>
#include <QtXml/QDomDocument>

#include <QHash>
#include <network/downloadimage.h>
#include <QFile>

struct Product{
    QString Description;
    QString ImageUrl;
    QString NaiadProductId;
    QString Name;
    QStringList Platforms;
    QString ProcessingCenters;
    QString ProcessingLevels;
    QString ProductionInterval;
    QStringList Sensors;
    QString SpatialResolutions;

    bool IsGlobalCoverage;
    bool IsMapModeSupported;
    bool IsOnGoing;
    bool IsPresentationModeSupported;
    bool IsPrivate;
    bool NoQuiklooks;

    QHash<QString, int> productsParameters;

    QDateTime StartDate;
    QDateTime EndDate;
    int FilteredGranulesCount;
    int Id;
    int TotalGranulesCount;
};

struct selectedProduct{
    QString productName;
    QDateTime startDate;
    QDateTime endDate;
    float west;
    float east;
    float south;
    float north;
    QString parameter;
};

class ProductsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProductsWidget(QWidget *parent = 0);
    void setSelectedProducts(QHash<QString, selectedProduct>* _selectedProducts);

protected:
    QVBoxLayout* vLayout;
    QDomDocument* dom;

    QString serverName;
    QUrl url;
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

    QDate* startProductDateValue;
    QTime* startProductTimeValue;
    QDateEdit* productDateStart;
    QDateEdit* productDateEnd;

    QDate* endProductDateValue;
    QTime* endProductTimeValue;
    QTimeEdit* productTimeStart;
    QTimeEdit* productTimeEnd;

    QLabel* parametersLbl;
    QComboBox* comboParameters;

    QPushButton* addProductLabel;

    QHash<QString, int> parametersList;
    QHash<QString, Product> productsHash;

    QHash<QString, selectedProduct>* selectedProducts;
signals:
    
public slots:
    void currentProductChanged(int index);
    void slotReadyRead();
    void getError(QNetworkReply::NetworkError);
    void reloadProductsList();
    void addProduct();
    void slotProductInfo();
};

#endif // PRODUCTSWIDGET_H
