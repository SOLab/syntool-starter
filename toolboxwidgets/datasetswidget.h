#ifndef DATASETSWIDGET_H
#define DATASETSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QDebug>

#include <additionalwidgets/datasetboxwidget.h>
#include <more/ProductStructures.h>

class DatasetsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DatasetsWidget(QWidget *parent = 0);

    void setGranules(QHash<QString, Granule>* granulesHash);
protected:
    QVBoxLayout* vLayout;
    QCheckBox* showAllCheck;
    QList<qint32> currentDatasets;

    QHash<QString, Granule>* _granulesHash;

signals:
    void closeAllDatasetBoxWidgets();
    void closeDatasetForGranuleId(qint32 granuleId);
    
public slots:
    void addDatasets(QList<qint32> displayedGranules);
    
};

#endif // DATASETSWIDGET_H
