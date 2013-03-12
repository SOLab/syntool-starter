#ifndef GRANULEINFOWIDGET_H
#define GRANULEINFOWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include "more/ProductStructures.h"
#include <QDebug>
#include <QLabel>
#include <QPushButton>

class GranuleInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GranuleInfoWidget(Granule granule, QWidget *parent = 0);
    
protected:
    QVBoxLayout* vLayout;

signals:
    
public slots:
    
};

#endif // GRANULEINFOWIDGET_H
