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
    QLabel* projections = new QLabel("Projections");
    vLayout->addWidget(projections);

    supportedProjections = new QComboBox;
    QStringList projList;
    projList << "Globe" << "Flat Map" << "Mercator";
    supportedProjections->addItems(projList);
    vLayout->addWidget(supportedProjections);

    QLabel* themesLbl = new QLabel("Themes");
    vLayout->addWidget(themesLbl);

    themesArea = new QScrollArea(this);

    QWidget* themesWgt = new QWidget(this);
    QVBoxLayout* themesLayout = new QVBoxLayout(themesWgt);

    firstTheme = new ImageButton(":/osm.png", "OpenStreetMap", themesWgt);
    secondTheme = new ImageButton(":/icons/picture.png", "Satellite View", themesWgt);

    themesLayout->addWidget(firstTheme);
    themesLayout->addWidget(secondTheme);

    themesLayout->setContentsMargins(0,8,0,0);

    themesArea->setWidget(themesWgt);

    vLayout->addWidget(themesArea);
}
