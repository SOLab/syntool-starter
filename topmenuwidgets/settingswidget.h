#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QDebug>
#include "topmenuwidgets/settingstabwidget.h"



class SettingsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget *parent = 0);

protected:
    QVBoxLayout* vLayout;
    SettingsTabWidget* mainTabWidget;

    QPushButton* clearConfigButton;

    QWidget* buttonsWidget;
    QHBoxLayout* buttonsLayout;
    QPushButton* okButton;
    QPushButton* applyButton;
    QPushButton* cancelButton;

    void createForm();

signals:
    
public slots:
    void saveChanges();
    void clearConfig();
    
};

#endif // SETTINGWIDGET_H
