#include "datasetswidget.h"

DatasetsWidget::DatasetsWidget(ConfigData *configData, QWidget *parent) :
    QWidget(parent)
{
    serverName = configData->serverName;

    currentDatasets = new QHash<qint32, qint32>;
    currentRemoveNumbers = new QList<qint32>;
    selectedGranuleList = new QList<qint32>;
    hiddenProducts = new QList<qint32>;
    productTransparency = new QMap<qint32, qint32>;

    vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setAlignment(Qt::AlignTop);
    vLayout->setSpacing(3);

    setObjectName("DatasetsWidget");
    setStyleSheet(QString("QWidget#DatasetsWidget {background-color: "
                  "rgb(%1, %2, %3);}")
                  .arg(parent->palette().background().color().red())
                  .arg(parent->palette().background().color().green())
                  .arg(parent->palette().background().color().blue()));

    QHBoxLayout* hLayout = new QHBoxLayout;

    showAllCheck = new QCheckBox(this);
    showAllCheck->setText(tr("Show all"));
    showAllCheck->setChecked(true);

    hLayout->addWidget(showAllCheck);

    QPushButton* upButton = new QPushButton(this);
    upButton->setIcon(QIcon(":/icons/go_up.png"));
    upButton->setFlat(true);
    upButton->setFocusPolicy(Qt::NoFocus);
    upButton->setStyleSheet("QPushButton:pressed {\
                             background-color: #EEEEEE;\
                             border: 0;}");
    upButton->setIconSize(QSize(16, 16));
    upButton->setFixedSize(20, 20);
    connect(upButton, &QPushButton::clicked, this, &DatasetsWidget::upDatasetSlot);

    QPushButton* downButton = new QPushButton(this);
    downButton->setIcon(QIcon(":/icons/go_down.png"));
    downButton->setFlat(true);
    downButton->setFocusPolicy(Qt::NoFocus);
    downButton->setStyleSheet("QPushButton:pressed {\
                               background-color: #EEEEEE;\
                               border: 0;}");
    downButton->setIconSize(QSize(16, 16));
    downButton->setFixedSize(20, 20);
    connect(downButton, &QPushButton::clicked, this, &DatasetsWidget::downDatasetSlot);

    hLayout->addWidget(upButton);
    hLayout->addWidget(downButton);
    hLayout->setSpacing(0);

    vLayout->addLayout(hLayout);

    downloadAllButton = new QPushButton(tr("Download all"), this);
    downloadAllButton->setToolTip(tr("Download all selected granules"));
    connect(downloadAllButton, &QPushButton::clicked, this, &DatasetsWidget::downloadAllSlot);
    vLayout->addWidget(downloadAllButton);

    QFrame* hLine = new QFrame(this);
    hLine->setFrameShape(QFrame::HLine);
    hLine->setFrameShadow(QFrame::Sunken);
    vLayout->addWidget(hLine);

    datasetsLayout = new QVBoxLayout;
    vLayout->addLayout(datasetsLayout);

    granuleHeightMap = new QMap<qint32, qint32>;
    heightDatasetBoxMap = new QMap<qint32, DatasetBoxWidget*>;

    selectedDataset = NULL;
    selectedGranuleId = 0;

    maxHeight = configData->numberCachedSimpleGranules + configData->numberCachedTiledGranules + 50;

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

void DatasetsWidget::setGranules(QHash<QString, Granule> *granulesHash)
{
    m_granulesHash = granulesHash;
}

void DatasetsWidget::addDatasets(QHash<qint32, qint32> *displayedGranules)
{
    QHash<qint32, qint32>::const_iterator dgi = displayedGranules->constBegin();
    while (dgi != displayedGranules->constEnd()) {
        if (!currentDatasets->contains(dgi.key()))
        {
            // calculate height for new granule
            if (currentHeight >= maxHeight)
                currentHeight = 1;

            while (granuleHeightMap->contains(currentHeight))
            {
                if (currentHeight < maxHeight)
                    currentHeight++;
                else
                    currentHeight = 1;
            }

            currentDatasets->insert(dgi.key(), dgi.value());

            DatasetBoxWidget* datasetBox = new DatasetBoxWidget(serverName,
                                           m_granulesHash->value(QString::number(dgi.key())), this);

            if (showAllCheck->isChecked() && !hiddenProducts->contains(dgi.value()))
            {
                datasetBox->setChecked(true);
                if (!selectedGranuleList->contains(dgi.key()))
                    selectedGranuleList->append(dgi.key());
                emit displayGranule(dgi.key(), dgi.value(), currentHeight);
            }

            connect(datasetBox, &DatasetBoxWidget::granulePropertiesSignal, this, &DatasetsWidget::actionPropertiesSlot);

            connect(this, &DatasetsWidget::closeDatasetForGranuleId, datasetBox, &DatasetBoxWidget::closeGranuleId);
            connect(showAllCheck, &QCheckBox::clicked, datasetBox, &DatasetBoxWidget::setChecked);

            // showDatasetCheck clicked
            connect(datasetBox, &DatasetBoxWidget::changedDisplayGranule, this, &DatasetsWidget::changedDisplayGranule);

            // changed transparency
            connect(datasetBox, &DatasetBoxWidget::changedTransparency, this, &DatasetsWidget::changedGranuleTransparency);
            // select new dataset
            connect(datasetBox, &DatasetBoxWidget::selectDataset, this, &DatasetsWidget::selectDatasetSlot);

            qint32 index = datasetsLayout->indexOf(heightDatasetBoxMap->lowerBound(currentHeight).value());
            if (index < 0)
                index = datasetsLayout->count();

            // add elements to QMap
            granuleHeightMap->insert(dgi.key(), currentHeight);
            heightDatasetBoxMap->insert(currentHeight, datasetBox);

            datasetsLayout->insertWidget(index, datasetBox);
            currentHeight++;
        }
        ++dgi;
    }

    currentRemoveNumbers->clear();

    QHash<qint32, qint32>::const_iterator cdi = currentDatasets->constBegin();
    while (cdi != currentDatasets->constEnd()) {
        if (!displayedGranules->contains(cdi.key()))
        {
            emit closeDatasetForGranuleId(cdi.key());
            currentRemoveNumbers->append(cdi.key());
            emit hideGranule(cdi.key(), cdi.value());
            selectedGranuleList->removeAll(cdi.key());
            // remove from QMap
            heightDatasetBoxMap->remove(granuleHeightMap->value(cdi.key()));
            granuleHeightMap->remove(cdi.key());

            if (selectedGranuleId == cdi.key())
            {
                selectedDataset = NULL;
                selectedGranuleId = 0;
            }
        }
        ++cdi;
    }

    for (int removeNumber = 0; removeNumber < currentRemoveNumbers->size(); ++removeNumber)
    {
        currentDatasets->remove(currentRemoveNumbers->at(removeNumber));
    }
}

void DatasetsWidget::actionPropertiesSlot(qint32 granuleId)
{
        GranuleInfoWidget* currentGranuleWidget =
                new GranuleInfoWidget(m_granulesHash->value(QString::number(granuleId)));
        currentGranuleWidget->show();
}

void DatasetsWidget::changedDisplayGranule(bool checked, qint32 granuleId, qint32 productId)
{
    if (checked)
    {
        emit displayGranule(granuleId, productId, granuleHeightMap->value(granuleId));
        if (!selectedGranuleList->contains(granuleId))
            selectedGranuleList->append(granuleId);
    }
    else
    {
        emit hideGranule(granuleId, productId);
        selectedGranuleList->removeAll(granuleId);
    }
}

void DatasetsWidget::downloadAllSlot()
{
    for (int i = 0; i < selectedGranuleList->size(); ++i) {
         granuleActions(serverName, QString::number(selectedGranuleList->at(i)), "ftp");
         qCritical() << "DOWNLOAD GRANULE: "<< selectedGranuleList->at(i);
    }
}

void DatasetsWidget::selectDatasetSlot(qint32 granuleId)
{
    DatasetBoxWidget* tempDataset = qobject_cast<DatasetBoxWidget *>(sender());
    if (selectedDataset)
        selectedDataset->setBold(false);
    tempDataset->setBold(true);
    selectedGranuleId = granuleId;
    selectedDataset = tempDataset;
}

void DatasetsWidget::upDatasetSlot()
{
    if (!selectedDataset)
        return;
    qint32 selectedIndex = datasetsLayout->indexOf(selectedDataset);
    if (selectedIndex < 1)
        return;

    qint32 newIndex = selectedIndex - 1;

    qint32 selectedHeight = granuleHeightMap->value(selectedDataset->granuleId());
    qint32 granuleIdSelectedDataset = selectedDataset->granuleId();

    QMap<qint32, DatasetBoxWidget*>::const_iterator i = heightDatasetBoxMap->find(selectedHeight);
    i--;
    qint32 granuleIdSecondDataset = i.value()->granuleId();

    qint32 newHeight = granuleHeightMap->value(granuleIdSecondDataset);

    granuleHeightMap->remove(granuleIdSelectedDataset);
    granuleHeightMap->remove(granuleIdSecondDataset);

    granuleHeightMap->insert(granuleIdSelectedDataset, newHeight);
    granuleHeightMap->insert(granuleIdSecondDataset, selectedHeight);

    heightDatasetBoxMap->remove(selectedHeight);
    heightDatasetBoxMap->insert(selectedHeight, heightDatasetBoxMap->value(newHeight));

    heightDatasetBoxMap->remove(newHeight);
    heightDatasetBoxMap->insert(newHeight, selectedDataset);

    emit changedGranulesHeight(granuleIdSelectedDataset, newHeight, granuleIdSecondDataset, selectedHeight);

    datasetsLayout->removeWidget(selectedDataset);
    datasetsLayout->insertWidget(newIndex, selectedDataset);
}

void DatasetsWidget::downDatasetSlot()
{
    if (!selectedDataset)
        return;
    qint32 selectedIndex = datasetsLayout->indexOf(selectedDataset);

    qint32 newIndex = selectedIndex + 1;
    if (newIndex >= heightDatasetBoxMap->count())
        return;

    qint32 selectedHeight = granuleHeightMap->value(selectedDataset->granuleId());
    qint32 granuleIdSelectedDataset = selectedDataset->granuleId();

    QMap<qint32, DatasetBoxWidget*>::const_iterator i = heightDatasetBoxMap->find(selectedHeight);
    i++;
    qint32 granuleIdSecondDataset = i.value()->granuleId();

    qint32 newHeight = granuleHeightMap->value(granuleIdSecondDataset);

    granuleHeightMap->remove(granuleIdSelectedDataset);
    granuleHeightMap->remove(granuleIdSecondDataset);
    granuleHeightMap->insert(granuleIdSelectedDataset, newHeight);
    granuleHeightMap->insert(granuleIdSecondDataset, selectedHeight);

    heightDatasetBoxMap->remove(selectedHeight);
    heightDatasetBoxMap->insert(selectedHeight, heightDatasetBoxMap->value(newHeight));

    heightDatasetBoxMap->remove(newHeight);
    heightDatasetBoxMap->insert(newHeight, selectedDataset);

    emit changedGranulesHeight(granuleIdSelectedDataset, newHeight, granuleIdSecondDataset, selectedHeight);

    datasetsLayout->removeWidget(selectedDataset);
    datasetsLayout->insertWidget(newIndex, selectedDataset);
}

void DatasetsWidget::setShowProduct(QString productName, qint32 productId, qint32 showState)
{
    Q_UNUSED(productName);
    if (showState == Qt::Unchecked)
    {
        hiddenProducts->append(productId);

        QHash<qint32, qint32>::iterator i;
        for (i = currentDatasets->begin(); i != currentDatasets->end(); ++i)
        {
            if (i.value() == productId)
            {
                qint32 _height = granuleHeightMap->value(i.key());
                heightDatasetBoxMap->value(_height)->setChecked(false);
            }
        }
    }
    else
    {
        hiddenProducts->removeAll(productId);

        QHash<qint32, qint32>::iterator i;
        for (i = currentDatasets->begin(); i != currentDatasets->end(); ++i)
        {
            if (i.value() == productId)
            {
                qint32 _height = granuleHeightMap->value(i.key());
                heightDatasetBoxMap->value(_height)->setChecked(true);
            }
        }
    }
}

void DatasetsWidget::changedProductTransparency(QString productName, qint32 productId, int value)
{
    Q_UNUSED(productName);
    productTransparency->insert(productId, value);

    QHash<qint32, qint32>::iterator i;
    for (i = currentDatasets->begin(); i != currentDatasets->end(); ++i)
    {
        if (i.value() == productId)
        {
            emit changedGranuleTransparency(i.key(), productId, value);
        }
    }
}
