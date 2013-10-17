#ifndef STARTINFOWIDGET_H
#define STARTINFOWIDGET_H

#include <QDialog>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QImageReader>
#include <QSettings>
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
    QCheckBox* hideCheckBox;
    QPushButton* closeButton;
    QWidget* wgt;

    ConfigData* _configData;
    
signals:
    
public slots:
    
protected:
    void closeEvent(QCloseEvent *ev);
};

#endif // STARTINFOWIDGET_H
