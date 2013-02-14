#include "datasetboxwidget.h"

DatasetBoxWidget::DatasetBoxWidget(Granule granule, QWidget *parent):
    QWidget(parent)
{
    gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);

    showDatasetCheck = new QCheckBox(this);

    granuleNameLabel = new QLabel(granule.granuleName, this);
    granuleNameLabel->setWordWrap(true);
    granuleNameLabel->setMinimumSize(1,1);
    granuleNameLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    _granuleId = granule.granuleId;

    transparencySlider = new QSlider(Qt::Horizontal, this);
    transparencySlider->setMaximum(100);
    transparencySlider->setValue(100);
    transparencySlider->setToolTip(tr("Transparency layer"));

    percentLabel = new QLabel("100%", this);
    percentLabel->setFixedWidth(percentLabel->sizeHint().width());

    connect(transparencySlider, &QSlider::valueChanged, this, &DatasetBoxWidget::changedTransparencySlot);

    gridLayout->setColumnStretch(0, 0);
    gridLayout->setColumnStretch(1, 10);
    gridLayout->setColumnStretch(2, 0);
    gridLayout->addWidget(showDatasetCheck, 0,0);
    gridLayout->addWidget(granuleNameLabel, 0, 1, 2, 3, Qt::AlignLeft);

    gridLayout->addWidget(transparencySlider, 2,0, 1, 3);
    gridLayout->addWidget(percentLabel, 2,3);

    resize(sizeHint());

    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
}

void DatasetBoxWidget::setChecked(bool checked)
{
    showDatasetCheck->setChecked(checked);
}

void DatasetBoxWidget::closeGranuleId(qint32 granuleId)
{
    if (_granuleId == granuleId)
    {
        hide();
        deleteLater();
    }
}

void DatasetBoxWidget::changedTransparencySlot(qint32 transparentValue)
{
    percentLabel->setText(QString::number(transparentValue)+"%");
    emit changedTransparency(_granuleId, transparentValue);
}
