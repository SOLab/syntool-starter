#ifndef DATASETSWIDGET_H
#define DATASETSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QDebug>

#include "additionalwidgets/datasetboxwidget.h"
#include "more/ProductStructures.h"
#include "more/structure.h"

class DatasetsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DatasetsWidget(ConfigData configData, QWidget *parent = 0);

    void setGranules(QHash<QString, Granule>* granulesHash);
protected:
    QVBoxLayout* vLayout;
    QCheckBox* showAllCheck;
    QList<qint32> currentDatasets;

    QHash<QString, Granule>* _granulesHash;
    QString serverName;

signals:
    void closeAllDatasetBoxWidgets();
    void closeDatasetForGranuleId(qint32 granuleId);
    
public slots:
    void addDatasets(QList<qint32> displayedGranules);
    void actionPropertiesSlot(qint32 granuleId);
    
};

#endif // DATASETSWIDGET_H
