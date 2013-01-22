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
#include <toolboxwidgets/inputbox.h>

#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QDebug>
#include <QPushButton>
#include <QtXml/QDomDocument>

#include <QHash>

class ProductsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProductsWidget(QWidget *parent = 0);

protected:
    QVBoxLayout* vLayout;
    QDomDocument* dom;

    QUrl url;
    QNetworkAccessManager *networkManager;
//    QNetworkReply *reply;
    QByteArray currentRequest;
    QLabel* productsLbl;
    QComboBox* comboProducts;
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

    QHash<QString, QStringList> productsParametersList;
signals:
    
public slots:
    void currentProductChanged(int index);
    void slotReadyRead();
    void getError(QNetworkReply::NetworkError);
    void reloadProductsList();
};

#endif // PRODUCTSWIDGET_H
