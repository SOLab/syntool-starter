#ifndef PRODUCTSWIDGET_H
#define PRODUCTSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>

class ProductsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProductsWidget(QWidget *parent = 0);
    
protected:
    QVBoxLayout* vLayout;
    QComboBox* comboProducts;

signals:
    
public slots:
    
};

#endif // PRODUCTSWIDGET_H
