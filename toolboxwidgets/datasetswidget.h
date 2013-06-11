#ifndef DATASETSWIDGET_H
#define DATASETSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QDebug>
#include <QPushButton>

#include "additionalwidgets/datasetboxwidget.h"
#include "more/ProductStructures.h"
#include "more/structure.h"
#include "more/granuleactions.h"

class DatasetsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DatasetsWidget(ConfigData *configData, QWidget *parent = 0);

    void setGranules(QHash<QString, Granule>* granulesHash);
protected:
    QVBoxLayout*             vLayout;
    QVBoxLayout*             datasetsLayout;
    QCheckBox*               showAllCheck;
    QPushButton*             downloadAllButton;
    // <granuleId, productId>
    QHash<qint32, qint32>*   currentDatasets;
    QList<qint32>*           currentRemoveNumbers;
    QList<qint32>*           selectedGranuleList;

    QHash<QString, Granule>* m_granulesHash;
    QString                  serverName;
    DatasetBoxWidget*        selectedDataset;
    qint32                   selectedGranuleId;

    // <granuleId, height> - QMap items are always sorted by key.
    QMap<qint32, qint32>*            heightGranuleMap;
    // <height, datasetBox>
    QMap<qint32, DatasetBoxWidget*>* heightDatasetBoxMap;
    qint32                           currentHeight;
    qint32                           maxHeight;

signals:
    void closeAllDatasetBoxWidgets();
    void closeDatasetForGranuleId(qint32 granuleId);

    void displayGranule(qint32 granuleId, qint32 productId, qint32 height, GranuleType::Type type = GranuleType::Simple);
    void hideGranule(qint32 granuleId, qint32 productId);

    void changedGranuleTransparency(qint32 granuleId, qint32 productId, qint32 transparency);

    void changedGranuleHeight(qint32 granuleId, qint32 height);
    void changedGranulesHeight(qint32 granuleId1, qint32 height1, qint32 granuleId2, qint32 height2);
    
public slots:
    void addDatasets(QHash<qint32, qint32>* displayedGranules);
    void actionPropertiesSlot(qint32 granuleId);
    void changedDisplayGranule(bool checked, qint32 granuleId, qint32 productId);
    void downloadAllSlot();

    void selectDatasetSlot(qint32 granuleId);
    void upDatasetSlot();
    void downDatasetSlot();
};

#endif // DATASETSWIDGET_H
