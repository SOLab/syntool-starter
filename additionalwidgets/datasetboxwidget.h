#ifndef DATASETBOXWIDGET_H
#define DATASETBOXWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QSlider>

#include <QDebug>

#include <more/ProductStructures.h>

class DatasetBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DatasetBoxWidget(Granule granule, QWidget *parent = 0);


protected:
    QGridLayout* gridLayout;
    QCheckBox* showDatasetCheck;
    QLabel* granuleNameLabel;
    QSlider* transparencySlider;
    QLabel* percentLabel;

    qint32 _granuleId;

signals:
    void changedTransparency(qint32 _granuleId, qint32 transparentValue);

public slots:
    void closeGranuleId(qint32 granuleId);
    void changedTransparencySlot(qint32 transparentValue);

    void setChecked(bool checked);
};

#endif // DATASETBOXWIDGET_H
