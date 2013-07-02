#ifndef LAYERSWIDGET_H
#define LAYERSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

#include "additionalwidgets/layerboxwidget.h"
#include "more/structure.h"

class LayersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LayersWidget(QWidget *parent = 0);

protected:
    QVBoxLayout* vLayout;
    QVBoxLayout* productsLayout;
    QVBoxLayout* favoritesLayout;
    QLabel*      productLayersLbl;
    QLabel*      favoritesLayersLbl;

    QList<QString>* currentProducts;

signals:
    void removeLayer(QString productId);
    void showLayer(QString productName, qint32 productId, qint32 showState);
    void changedProductTransparency(QString productName, qint32 productId, int value);

    void removeProductSignal(QString productId);
    
public slots:
    void addProduct(QString ProductNaiadId, qint32 productId, ProductType::Type productType);
    void deleteProduct(QString ProductId);
    
};

#endif // LAYERSWIDGET_H
