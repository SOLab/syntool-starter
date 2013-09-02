#ifndef STARTINFOWIDGET_H
#define STARTINFOWIDGET_H

#include <QDialog>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QImageReader>
#include <QDebug>

#include "more/structure.h"

class MainWindow;

class StartInfoWidget : public QDialog
{
    Q_OBJECT
public:
    explicit StartInfoWidget(ConfigData* configData, QWidget* mainWindow, QWidget *parent = 0);
    QGridLayout* gridLayout;
    QVBoxLayout* mainLayout;
    QLabel* label;
    QLabel* imageLabel;
    QPushButton* button;
    QWidget* wgt;
    
signals:
    
public slots:
    
};

#endif // STARTINFOWIDGET_H
