#ifndef PRODUCTSWIDGET_H
#define PRODUCTSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QDate>
#include <QTime>
#include <QDateEdit>
#include <QTimeEdit>
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

    QDate* startProductDateValue;
    QTime* startProductTimeValue;
    QDateEdit* productDateStart;
    QDateEdit* productDateEnd;

    QDate* endProductDateValue;
    QTime* endProductTimeValue;
    QTimeEdit* productTimeStart;
    QTimeEdit* productTimeEnd;
signals:
    
public slots:
    void currentProductChanged(int index);
    
};

#endif // PRODUCTSWIDGET_H
