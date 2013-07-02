#include "layerswidget.h"

LayersWidget::LayersWidget(QWidget *parent) :
    QWidget(parent)
{
    currentProducts = new QList<QString>;

    vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0,2,0,0);
    vLayout->setSpacing(3);
    vLayout->setAlignment(Qt::AlignTop);

    productsLayout = new QVBoxLayout;
    productsLayout->setContentsMargins(0,0,0,0);
    productsLayout->setAlignment(Qt::AlignTop);

    favoritesLayout = new QVBoxLayout;
    favoritesLayout->setContentsMargins(0,0,0,0);
    favoritesLayout->setAlignment(Qt::AlignTop);

    setObjectName("LayersWidget");
    setStyleSheet(QString("QWidget#LayersWidget {background-color: "
                  "rgb(%1, %2, %3);}")
                  .arg(parent->palette().background().color().red())
                  .arg(parent->palette().background().color().green())
                  .arg(parent->palette().background().color().blue()));

    productLayersLbl = new QLabel(tr("Products:"), this);
    vLayout->addWidget(productLayersLbl);
    vLayout->addLayout(productsLayout);

    QFrame* hLine = new QFrame(this);
    hLine->setFrameShape(QFrame::HLine);
    hLine->setFrameShadow(QFrame::Sunken);
    vLayout->addWidget(hLine);

    favoritesLayersLbl = new QLabel(tr("Favorites:"), this);
    vLayout->addWidget(favoritesLayersLbl);
    vLayout->addLayout(favoritesLayout);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    connect(this, &LayersWidget::removeLayer, this, &LayersWidget::deleteProduct);
}

void LayersWidget::addProduct(QString ProductNaiadId, qint32 productId, ProductType::Type productType)
{
    if (!currentProducts->contains(ProductNaiadId))
    {
        LayerBoxWidget* layerBox = new LayerBoxWidget(ProductNaiadId, productId, this);

        // connect signals with signals
        connect(layerBox, &LayerBoxWidget::removeLayer, this, &LayersWidget::removeLayer);
        connect(layerBox, &LayerBoxWidget::showLayer, this, &LayersWidget::showLayer);
        connect(layerBox, &LayerBoxWidget::changedTransparency, this, &LayersWidget::changedProductTransparency);

        // other connects
        connect(this, &LayersWidget::removeProductSignal, layerBox, &LayerBoxWidget::removeLayerBox);

        if (productType == ProductType::Product)
            productsLayout->addWidget(layerBox);
        else if (productType == ProductType::Favorite)
            favoritesLayout->addWidget(layerBox);
        currentProducts->append(ProductNaiadId);
    }
}

void LayersWidget::deleteProduct(QString ProductId)
{
    if (currentProducts->contains(ProductId))
    {
        currentProducts->removeOne(ProductId);
        emit removeProductSignal(ProductId);
    }
}
