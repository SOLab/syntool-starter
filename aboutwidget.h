#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#include <QDialog>
#include <QLabel>
#include <QDate>
#include <QImageReader>
#include <QHBoxLayout>
#include "more/structure.h"

class AboutWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AboutWidget(ConfigData *configData, QWidget *parent = 0);
    
protected:
    QLabel*      statusLabel;
    QLabel*      imageLabel;
    QHBoxLayout* topLayout;
    
public slots:
    
};

#endif // ABOUTWIDGET_H
