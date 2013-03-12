#ifndef PRODUCTINFOWIDGET_H
#define PRODUCTINFOWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include "toolboxwidgets/productswidget.h"
//#include <QPixmap>

class ProductInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProductInfoWidget(QWidget *parent = 0);
    void setProduct(struct Product product, QPixmap *productImagePixmap);

protected:
    QGridLayout* gridLayout;
    QLabel* Description;
    QLabel* StartDate;
    QLabel* EndDate;
    QLabel* NaiadProductId;
    QLabel* Name;

private slots:
    void closeEvent(QCloseEvent *event);

signals:
    
public slots:
    
};

#endif // PRODUCTINFOWIDGET_H
