#ifndef PLACEWIDGET_H
#define PLACEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class PlaceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PlaceWidget(QWidget *parent = 0);

protected:
    QVBoxLayout* vLayout;
signals:
    
public slots:
    
};

#endif // PLACEWIDGET_H
