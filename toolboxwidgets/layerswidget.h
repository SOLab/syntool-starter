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
    QLabel* productLayersLbl;
    QLabel* othersLayersLbl;

signals:
    
public slots:
    
};

#endif // LAYERSWIDGET_H
