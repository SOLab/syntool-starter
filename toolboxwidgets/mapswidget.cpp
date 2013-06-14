#include "mapswidget.h"

MapsWidget::MapsWidget(QWidget *parent) :
    QWidget(parent)
{
    vLayout = new QVBoxLayout(this);
    vLayout->setContentsMargins(0,0,0,0);

    setObjectName("MapsWidget");
    setStyleSheet(QString("QWidget#MapsWidget {background-color: "
                  "rgb(%1, %2, %3);}")
                  .arg(parent->palette().background().color().red())
                  .arg(parent->palette().background().color().green())
                  .arg(parent->palette().background().color().blue()));

    vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    QLabel* projections = new QLabel(tr("Projections"));
    vLayout->addWidget(projections);

    supportedProjections = new QComboBox;
    QStringList projList;
    projList << tr("Globe") << tr("Flat Map") << tr("Mercator");
    supportedProjections->addItems(projList);
    vLayout->addWidget(supportedProjections);

    QLabel* themesLbl = new QLabel(tr("Themes"));
    vLayout->addWidget(themesLbl);

    themesArea = new QScrollArea(this);
    themesArea->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    QWidget* themesWgt = new QWidget(this);
    QVBoxLayout* themesLayout = new QVBoxLayout(themesWgt);

    OSMTheme = new ImageButton(":/osm.png", "OpenStreetMap", themesWgt);
    OSMTheme->setObjectName("OSM");
    connect(OSMTheme, &ImageButton::clicked, this, &MapsWidget::changeStyle);

    transportOSMTheme = new ImageButton(":/icons/transport.png", tr("Transport View"), themesWgt);
    transportOSMTheme->setObjectName("transportOSM");
    connect(transportOSMTheme, &ImageButton::clicked, this, &MapsWidget::changeStyle);

    yandexSatelliteTheme = new ImageButton(":/icons/picture.png", tr("Yandex Satellite"), themesWgt);
    yandexSatelliteTheme->setObjectName("yandexSatellite");
    connect(yandexSatelliteTheme, &ImageButton::clicked, this, &MapsWidget::changeStyle);

    themesLayout->addWidget(OSMTheme, 0, Qt::AlignHCenter);
    themesLayout->addWidget(transportOSMTheme, 0, Qt::AlignHCenter);
    themesLayout->addWidget(yandexSatelliteTheme, 0, Qt::AlignHCenter);

    themesLayout->setContentsMargins(0,8,0,0);

    themesArea->setWidget(themesWgt);

    vLayout->addWidget(themesArea);

    currentMapTheme = "OSM";
}

void MapsWidget::changeStyle()
{
    currentMapTheme = sender()->objectName();
    emit changeMapTheme(currentMapTheme);
}
