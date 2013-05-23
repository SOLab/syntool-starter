#include "datasetswidget.h"

DatasetsWidget::DatasetsWidget(ConfigData configData, QWidget *parent) :
    QWidget(parent)
{
    serverName = configData.serverName;

    currentDatasets = new QHash<qint32, qint32>;
    currentRemoveNumbers = new QList<qint32>;

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

    showAllCheck = new QCheckBox(this);
    showAllCheck->setText(tr("Show all"));
    showAllCheck->setChecked(true);

    vLayout->addWidget(showAllCheck);

    QFrame* hLine = new QFrame(this);
    hLine->setFrameShape(QFrame::HLine);
    hLine->setFrameShadow(QFrame::Sunken);
    vLayout->addWidget(hLine);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
}

void DatasetsWidget::setGranules(QHash<QString, Granule> *granulesHash)
{
    _granulesHash = granulesHash;
}

void DatasetsWidget::addDatasets(QHash<qint32, qint32> *displayedGranules)
{
    emit closeAllDatasetBoxWidgets();

    QHash<qint32, qint32>::const_iterator dgi = displayedGranules->constBegin();
    while (dgi != displayedGranules->constEnd()) {
        if (!currentDatasets->contains(dgi.key()))
        {
            currentDatasets->insert(dgi.key(), dgi.value());
            emit displayGranule(dgi.key(), dgi.value());

            DatasetBoxWidget* datasetBox = new DatasetBoxWidget(serverName,
                                           _granulesHash->value(QString::number(dgi.key())), this);
            datasetBox->setChecked(showAllCheck->isChecked());
            connect(datasetBox, &DatasetBoxWidget::granulePropertiesSignal,
                    this, &DatasetsWidget::actionPropertiesSlot);

            connect(this, &DatasetsWidget::closeDatasetForGranuleId,
                    datasetBox, &DatasetBoxWidget::closeGranuleId);
            connect(showAllCheck, &QCheckBox::clicked, datasetBox, &DatasetBoxWidget::setChecked);

            vLayout->addWidget(datasetBox);
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
                new GranuleInfoWidget(_granulesHash->value(QString::number(granuleId)));
        currentGranuleWidget->show();
}
