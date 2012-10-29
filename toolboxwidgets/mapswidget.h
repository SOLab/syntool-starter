#ifndef MAPSWIDGET_H
#define MAPSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class MapsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapsWidget(QWidget *parent = 0);

protected:
    QVBoxLayout* vLayout;
signals:
    
public slots:
    
};

#endif // MAPSWIDGET_H
