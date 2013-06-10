#ifndef DATASETBOXWIDGET_H
#define DATASETBOXWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QSlider>
#include <QPushButton>

#include <QDebug>
#include <QMouseEvent>

#include "more/ProductStructures.h"
#include "more/granuleactions.h"
#include "additionalwidgets/granuleinfowidget.h"

class DatasetBoxWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DatasetBoxWidget(QString serverName, Granule granule, QWidget *parent = 0);
    QCheckBox* showDatasetCheck;
    void setBold(bool value);

protected:
    QGridLayout*    gridLayout;
    QLabel*         granuleNameLabel;
    QSlider*        transparencySlider;
    QLabel*         percentLabel;
    QFrame*         hLine;

    QPushButton*    moreActions;

    QGridLayout*    buttonsHLayout;
    QWidget*        buttonsWidget;

    QPushButton*    imageButton;
    QPushButton*    opendapButton;
    QPushButton*    ftpButton;
    QPushButton*    kmlButton;
    QPushButton*    propertiesButton;
    bool            buttonsCreated;

    qint32          m_granuleId;
    qint32          m_productId;
    QString         m_serverName;

    void mousePressEvent (QMouseEvent * e);

signals:
    void changedTransparency(qint32 granuleId, qint32 productId, qint32 transparentValue);
    void granulePropertiesSignal(qint32 granuleId);
    void changedDisplayGranule(bool checked, qint32 granuleId, qint32 productId);
    void selectDataset(qint32 qranuleId);

public slots:
    void closeGranuleId(qint32 granuleId);
    void changedTransparencySlot(qint32 transparentValue);
    void showMoreButtons();

    void setChecked(bool checked);
    void checkedSlot(int state);

    void actionImageSlot();
    void actionOpendapSlot();
    void actionFtpSlot();
    void actionKmlSlot();
    void actionPropertiesSlot();
};

#endif // DATASETBOXWIDGET_H
