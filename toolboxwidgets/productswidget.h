#ifndef PRODUCTSWIDGET_H
#define PRODUCTSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <toolboxwidgets/inputbox.h>

class ProductsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProductsWidget(QWidget *parent = 0);
protected:
    QVBoxLayout* vLayout;
    QComboBox* comboProducts;

    InputBox* North;
    InputBox* South;
    InputBox* West;
    InputBox* East;
signals:
    
public slots:
    void currentProductChanged(int index);
    
};

#endif // PRODUCTSWIDGET_H
