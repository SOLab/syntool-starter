#ifndef LAYERSWIDGET_H
#define LAYERSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

#include "additionalwidgets/layerboxwidget.h"

class LayersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LayersWidget(QWidget *parent = 0);

protected:
    QVBoxLayout* vLayout;
    QVBoxLayout* productsLayout;
    QVBoxLayout* otherLayout;
    QLabel*      productLayersLbl;
    QLabel*      othersLayersLbl;

    QList<QString>* currentProducts;

signals:
    void removeLayer(QString productId);
    void showLayer(QString productName, qint32 productId, qint32 showState);
    void changedProductTransparency(QString productName, qint32 productId, int value);

    void removeLayerBox(QString productId);
    
public slots:
    void addProduct(QString ProductNaiadId, qint32 productId);
    void deleteProduct(QString ProductId);
    
};

#endif // LAYERSWIDGET_H
