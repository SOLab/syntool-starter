#ifndef MAPSWIDGET_H
#define MAPSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QScrollArea>
#include "additionalwidgets/imagebutton.h"

class MapsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapsWidget(QWidget *parent = 0);

protected:
    QVBoxLayout* vLayout;

    QComboBox* supportedProjections;
    QScrollArea* themesArea;
    ImageButton* firstTheme;
    ImageButton* secondTheme;
signals:
    
public slots:
    
};

#endif // MAPSWIDGET_H
