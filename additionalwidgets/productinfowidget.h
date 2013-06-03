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
    QLabel* description;
    QLabel* startDate;
    QLabel* endDate;
    QLabel* naiadProductId;
    QLabel* productId;
    QLabel* name;

private slots:
    void closeEvent(QCloseEvent *event);

signals:
    
public slots:
    
};

#endif // PRODUCTINFOWIDGET_H
