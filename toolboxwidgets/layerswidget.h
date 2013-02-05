#ifndef LAYERSWIDGET_H
#define LAYERSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

#include <additionalwidgets/layerboxwidget.h>

class LayersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LayersWidget(QWidget *parent = 0);

protected:
    QVBoxLayout* vLayout;
    QVBoxLayout* productsLayout;
    QVBoxLayout* otherLayout;
    QLabel* productLayersLbl;
    QLabel* othersLayersLbl;

signals:
    void removeLayer(QString productId);
    void showLayer(QString productId, int showState);
    void changedTransparency(QString productId, int value);
    
public slots:
    void addProduct(QString ProductId);
    
};

#endif // LAYERSWIDGET_H
