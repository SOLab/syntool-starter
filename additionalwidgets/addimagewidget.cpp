#include "addimagewidget.h"

AddImageWidget::AddImageWidget(ConfigData* configData, QWidget *parent) :
    QWidget(parent)
{
    _configData = configData;

    gridLayout = new QGridLayout(this);

    imageLabel = new QLabel(tr("PNG file"), this);
    imageWgt = new FileOpenWidget("file", tr("Image"), configData->cacheDir, this);
    connect(imageWgt, &FileOpenWidget::textChanged, this, &AddImageWidget::imageTextChangedSlot);

    gridLayout->addWidget(imageLabel, 0,0);
    gridLayout->addWidget(imageWgt, 0,1,1,5);

    kmlLabel = new QLabel(tr("KML file"), this);
    kmlWgt = new FileOpenWidget("file", tr("KML"), configData->cacheDir, this);

    imageWgt->setText("/mnt/d/distr/test_MER/MER.FRS.1PNEPA20100401.1542.VarBn_kml");
    kmlWgt->setText("/mnt/d/distr/test_MER/MER.FRS.1PNEPA20100401.1542.VarBn_kml.kml");

    gridLayout->addWidget(kmlLabel, 1,0);
    gridLayout->addWidget(kmlWgt, 1,1,1,5);

    zoomLabel = new QLabel(tr("Zoom"), this);
    dashLabel = new QLabel(" - ", this);

    lowZoom = new QSpinBox(this);
    lowZoom->setValue(3);
    lowZoom->setRange(0, 12);

    topZoom = new QSpinBox(this);
    topZoom->setValue(7);
    topZoom->setRange(0, 12);

    gridLayout->addWidget(zoomLabel, 2,0);
    gridLayout->addWidget(lowZoom, 2,1);
    gridLayout->addWidget(dashLabel, 2,2);
    gridLayout->addWidget(topZoom, 2,3);

    okButton = new QPushButton("Ok", this);
    okButton->setMaximumWidth(72);
    connect(okButton, &QPushButton::clicked, this, &AddImageWidget::checkFiles);
    closeButton = new QPushButton("Close", this);
    closeButton->setMaximumWidth(72);
    connect(closeButton, &QPushButton::clicked, this, &AddImageWidget::close);

    gridLayout->addWidget(okButton, 2,4);
    gridLayout->addWidget(closeButton, 2,5);
}

void AddImageWidget::imageTextChangedSlot()
{
    QFileInfo fi(imageWgt->text());
    imageWgt->setDefaultDir(fi.path());
    kmlWgt->setDefaultDir(fi.path());
}

void AddImageWidget::checkFiles()
{
    QString imagePath = imageWgt->text();
    if ( ! QFile::exists(imagePath))
    {
        qCritical() << QString("File %1 not found!").arg(imagePath);
        imageLabel->setStyleSheet("QLabel { color : red; }");
        return;
    }

    QString kmlPath = kmlWgt->text();
    if ( ! QFile::exists(kmlPath))
    {
        qCritical() << QString("File %1 not found!").arg(kmlPath);
        kmlLabel->setStyleSheet("QLabel { color : red; }");
        return;
    }

    qint32 lowZoomValue = lowZoom->value();
    qint32 topZoomValue = topZoom->value();
    if (lowZoomValue > topZoomValue)
    {
        lowZoomValue = topZoomValue;
        topZoomValue = lowZoom->value();
    }
    QString zoomStr = (lowZoomValue == topZoomValue) ? QString::number(lowZoomValue)
                              : QString("%1-%2").arg(lowZoomValue).arg(topZoomValue);

    QFileInfo fi(imagePath);
    granuleName = fi.fileName();

    QDir outDir(_configData->cacheDir);
    if (QDir(_configData->cacheDir+"/"+granuleName).exists())
    {
        tileCreatedSlot();
        close();
        return;
    }
    outDir.mkdir(granuleName);
    outDir.cd(granuleName);

    QString cmd = QString("python slbtiles.py --image %1 --kml %2 --zoom %3 -o %4")
            .arg(imagePath).arg(kmlPath).arg(zoomStr).arg(outDir.path());

    qDebug() << "==> " << cmd;

    tileProc = new QProcess(this);
    connect(tileProc, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(tileCreatedSlot(int,QProcess::ExitStatus)));

    tileProc->start(cmd);

    close();
}

void AddImageWidget::tileCreatedSlot(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::NormalExit)
    {
        emit tileCreatedSignal(granuleName);
    }
    else
    {
        qCritical() << QString("ExitCode: %1  Error create %2").arg(exitCode).arg(granuleName);
    }
}
