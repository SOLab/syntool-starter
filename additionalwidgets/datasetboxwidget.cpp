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

    moreActions = new QPushButton(QIcon(":/icons/down.png"), "", this);
    moreActions->setIconSize(QSize(16, 16));
    moreActions->setFixedSize(24, 20);
    connect(moreActions, &QPushButton::clicked, this, &DatasetBoxWidget::showMoreButtons);

    transparencySlider = new QSlider(Qt::Horizontal, this);
    transparencySlider->setMaximum(100);
    transparencySlider->setValue(100);
    transparencySlider->setToolTip(tr("Transparency layer"));

    percentLabel = new QLabel("100%", this);
    percentLabel->setFixedWidth(percentLabel->sizeHint().width());

    connect(transparencySlider, &QSlider::valueChanged, this, &DatasetBoxWidget::changedTransparencySlot);

    gridLayout->setColumnStretch(0, 0);
//    gridLayout->setColumnStretch(1, 10);
//    gridLayout->setColumnStretch(2, 0);
    gridLayout->addWidget(showDatasetCheck, 0,0);
    gridLayout->addWidget(granuleNameLabel, 0, 1, 2, 3, Qt::AlignLeft);

    gridLayout->addWidget(moreActions, 0, 4, 1,1,Qt::AlignRight);

    gridLayout->addWidget(transparencySlider, 3,0, 1, 4);
    gridLayout->addWidget(percentLabel, 3,4);

    hLine = new QFrame(this);
    hLine->setFrameShape(QFrame::HLine);
    hLine->setFrameShadow(QFrame::Sunken);
    gridLayout->addWidget(hLine, 4, 0, 1, 4);

    buttonsCreated = false;

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

void DatasetBoxWidget::showMoreButtons()
{
    if (!buttonsCreated)
    {
        buttonsWidget = new QWidget(this);
        buttonsHLayout = new QGridLayout(buttonsWidget);
        buttonsHLayout->setContentsMargins(0,0,0,0);
        buttonsHLayout->setSpacing(1);

        imageButton = new QPushButton(tr("Image"), this);
//        imageButton->setMaximumSize(48, 24);
        imageButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

        opendapButton = new QPushButton(tr("OPeNDAP"), this);
//        opendapButton->setMaximumSize(48, 24);
        opendapButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

        ftpButton = new QPushButton(tr("FTP"), this);
//        ftpButton->setMaximumSize(48, 24);
        ftpButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

        kmlButton = new QPushButton(tr("KML"), this);
//        kmlButton->setMaximumSize(48, 24);
        kmlButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

        propertiesButton = new QPushButton(tr("Info"), this);
//        propertiesButton->setMaximumSize(48, 24);
        propertiesButton->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

        buttonsHLayout->addWidget(imageButton, 0,0,1,6);
        buttonsHLayout->addWidget(opendapButton, 0,6,1,9);
        buttonsHLayout->addWidget(ftpButton, 1,0,1,5);
        buttonsHLayout->addWidget(kmlButton, 1,5,1,5);
        buttonsHLayout->addWidget(propertiesButton, 1,10,1,5);

        buttonsWidget->hide();
        buttonsWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

        gridLayout->addWidget(buttonsWidget, 2, 0, 1, 5);
        buttonsCreated = true;
    }

    buttonsWidget->setVisible(!buttonsWidget->isVisible());
}
