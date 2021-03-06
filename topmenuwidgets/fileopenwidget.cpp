#include "fileopenwidget.h"

FileOpenWidget::FileOpenWidget(QString type, QString caption, QString directory, QWidget *parent) :
    QWidget(parent)
{
    dialogType = type;
    dialogCaption = caption;
    dialogDir = directory;

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0, 1, 0, 1);

    lineEdit = new QLineEdit(this);
    connect(lineEdit, &QLineEdit::textChanged, this, &FileOpenWidget::textChanged);

    lineEdit->resize(lineEdit->sizeHint());

    QPushButton *toolButton = new QPushButton(this);
    toolButton->setCursor(Qt::ArrowCursor);
    toolButton->setFocusPolicy(Qt::NoFocus);

//    try
//        {toolButton->setIcon(QIcon::fromTheme("document-open"));}
//    catch (int)
        {toolButton->setText(tr("Edit"));}

    toolButton->setMaximumWidth(72);

    connect(toolButton, &QToolButton::clicked, this, &FileOpenWidget::fileDialog);

    hLayout->addWidget(lineEdit);
    hLayout->addWidget(toolButton);
    hLayout->setSpacing(1);

    if (type == "files")
        lineEdit->setReadOnly(true);

    data = "";
    dataList = QStringList("");
}

void FileOpenWidget::fileDialog()
{
    fd = new QFileDialog(this, dialogCaption, dialogDir);
    fd->setWindowModality(Qt::WindowModal);

    if (dialogType == "dir")
        data = fd->getExistingDirectory(this, dialogCaption, dialogDir, QFileDialog::ShowDirsOnly);
    if (dialogType == "file")
        data = fd->getOpenFileName(this, dialogCaption, dialogDir);
    if (dialogType == "files")
        dataList = fd->getOpenFileNames(this, dialogCaption, dialogDir);
    setLabel();
}

void FileOpenWidget::setLabel()
{
    if (!data.isEmpty())
        lineEdit->setText(data);
    else if (!dataList.isEmpty())
    {
        lineEdit->setText(dataList.join(", "));
    }
}

QString FileOpenWidget::text()
{
    return lineEdit->text();
}

void FileOpenWidget::setText(QString text)
{
    lineEdit->setText(text);
}

QString FileOpenWidget::value()
{
    if (!data.isEmpty())
        return data;
    else
        return text();
}

QStringList FileOpenWidget::valueList()
{
    if (!dataList.isEmpty())
        return dataList;
    else
        return text().split(",");
}

void FileOpenWidget::setDefaultDir(QString dir)
{
    dialogDir = dir;
}
