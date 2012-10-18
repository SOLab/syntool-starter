#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class AboutWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AboutWidget(QWidget *parent = 0);
    
protected:
    QLabel* statusLabel;
    QHBoxLayout *topLayout;

signals:
    
public slots:
    
};

#endif // ABOUTWIDGET_H
