#ifndef LAYERSWIDGET_H
#define LAYERSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class LayersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LayersWidget(QWidget *parent = 0);

protected:
    QVBoxLayout* vLayout;
signals:
    
public slots:
    
};

#endif // LAYERSWIDGET_H
