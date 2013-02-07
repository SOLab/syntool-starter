#ifndef DATASETSWIDGET_H
#define DATASETSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class DatasetsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DatasetsWidget(QWidget *parent = 0);

protected:
    QVBoxLayout* vLayout;
signals:
    
public slots:
    
};

#endif // DATASETSWIDGET_H
