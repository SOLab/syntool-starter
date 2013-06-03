#ifndef FILEOPENWIDGET_H
#define FILEOPENWIDGET_H

#include <QObject>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QPushButton>

class FileOpenWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileOpenWidget(QString type, QString caption, QString directory, QWidget *parent = 0);

    QString value();
    QString text();
    void setText(QString text);
    void setLabel();
    QStringList valueList();

protected:
    QString dialogType;
    QString dialogCaption;
    QString dialogDir;

    QLineEdit*   lineEdit;
    QFileDialog* fd;
    QString      data;
    QStringList  dataList;

signals:
    void textChanged();
    
public slots:
    void fileDialog();
    
};

#endif // FILEOPENWIDGET_H
