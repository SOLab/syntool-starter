#ifndef ADDIMAGEWIDGET_H
#define ADDIMAGEWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDebug>
#include <QProcess>
#include <QFileInfo>

#include "topmenuwidgets/fileopenwidget.h"
#include "more/structure.h"

class AddImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddImageWidget(ConfigData* configData, QWidget *parent = 0);

protected:
    QGridLayout*    gridLayout;
    QLabel*         imageLabel;
    FileOpenWidget* imageWgt;
    QLabel*         kmlLabel;
    FileOpenWidget* kmlWgt;

    QLabel*         zoomLabel;
    QSpinBox*       lowZoom;
    QLabel*         dashLabel;
    QSpinBox*       topZoom;

    QPushButton*    okButton;
    QPushButton*    closeButton;
    ConfigData*     _configData;

    QString         granuleName;
    QProcess*       tileProc;

signals:
    void tileCreatedSignal(QString granuleName);
    
public slots:
    void imageTextChangedSlot();
    void checkFiles();
    void tileCreatedSlot(int exitCode = 0, QProcess::ExitStatus exitStatus = QProcess::NormalExit);
    
};

#endif // ADDIMAGEWIDGET_H
