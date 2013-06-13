#ifndef LAYERBOXWIDGET_H
#define LAYERBOXWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QSlider>
#include <QGridLayout>

#include <QDebug>

class LayerBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LayerBoxWidget(QString ProductName, qint32 productId, QWidget *parent = 0);
    
protected:
    QGridLayout*  gridLayout;
    QPushButton*  removeLayerButton;
    QCheckBox*    showLayerCheckBox;
    QSlider*      transparencySlider;
    QLabel*       productNameLabel;
    QLabel*       percentLabel;

    QString       m_productName;
    qint32        m_productId;

signals:
    void removeLayer(QString productId);
    void showLayer(QString productName, qint32 productId, qint32 showState);
    void changedTransparency(QString productId, int value);
    
public slots:
    void changedTransparencySlot(int value);
    void removeLayerSlot();
    void showChangedSlot(int state);

    void removeLayerBox(QString productId);
};

#endif // LAYERBOXWIDGET_H
