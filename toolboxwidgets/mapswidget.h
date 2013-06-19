#ifndef MAPSWIDGET_H
#define MAPSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QScrollArea>
#include <QDebug>
#include "additionalwidgets/imagebutton.h"
#include "more/structure.h"

class MapsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapsWidget(ConfigData* configData, QWidget *parent = 0);

protected:
    QVBoxLayout* vLayout;

    QComboBox*   supportedProjections;
    QScrollArea* themesArea;
    ImageButton* OSMTheme;
    ImageButton* transportOSMTheme;
    ImageButton* yandexMapsTheme;
    ImageButton* yandexSatelliteTheme;
    ImageButton* googleMapsTheme;
    ImageButton* googleSatelliteTheme;

private:
    QString currentMapTheme;

signals:
    void changeMapTheme(QString mapTheme);
    
public slots:
    void changeStyle();
    
};

#endif // MAPSWIDGET_H
