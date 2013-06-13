#include "layerboxwidget.h"

LayerBoxWidget::LayerBoxWidget(QString ProductName, qint32 productId, QWidget *parent) :
    QWidget(parent)
{
    m_productName = ProductName;
    m_productId = productId;

    gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(0,0,0,0);
//    gridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    gridLayout->setAlignment(Qt::AlignTop);

    removeLayerButton = new QPushButton(QIcon(":/icons/delete.png"), "", this);
    removeLayerButton->setFixedSize(20, 20);
    removeLayerButton->setIconSize(QSize(12,12));
    removeLayerButton->setToolTip(tr("Remove layer"));
    connect(removeLayerButton, &QPushButton::clicked, this, &LayerBoxWidget::removeLayerSlot);
//    connect(removeLayerButton, &QPushButton::clicked, this, &LayerBoxWidget::close);

    showLayerCheckBox = new QCheckBox(this);
    showLayerCheckBox->setToolTip(tr("Show layer"));
    showLayerCheckBox->setCheckState(Qt::Checked);
    connect(showLayerCheckBox, &QCheckBox::stateChanged, this, &LayerBoxWidget::showChangedSlot);

    transparencySlider = new QSlider(Qt::Horizontal, this);
    transparencySlider->setMaximum(100);
    transparencySlider->setValue(100);
    transparencySlider->setToolTip(tr("Transparency layer"));
    connect(transparencySlider, &QSlider::valueChanged, this, &LayerBoxWidget::changedTransparencySlot);

    productNameLabel = new QLabel(ProductName, this);
    productNameLabel->setMinimumSize(1,1);
    productNameLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    productNameLabel->setToolTip(ProductName);

    percentLabel = new QLabel("100%", this);
    percentLabel->setFixedWidth(percentLabel->sizeHint().width());

    gridLayout->setColumnStretch(0, 0);
    gridLayout->setColumnStretch(1, 10);
    gridLayout->setColumnStretch(2, 0);
    gridLayout->addWidget(showLayerCheckBox, 0,0);
    gridLayout->addWidget(productNameLabel, 0, 1, 2, 2, Qt::AlignLeft);
    gridLayout->addWidget(removeLayerButton, 0,3, Qt::AlignRight);

    gridLayout->addWidget(transparencySlider, 2,0, 1, 3);
    gridLayout->addWidget(percentLabel, 2,3);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void LayerBoxWidget::changedTransparencySlot(int value)
{
    percentLabel->setText(QString::number(value)+"%");
    emit changedTransparency(m_productName, value);
}

void LayerBoxWidget::removeLayerSlot()
{
    emit removeLayer(m_productName);
    close();
}

void LayerBoxWidget::showChangedSlot(int state)
{
    emit showLayer(m_productName, m_productId, state);
}

void LayerBoxWidget::removeLayerBox(QString productId)
{
    Q_UNUSED(productId);
    hide();
    deleteLater();
}
