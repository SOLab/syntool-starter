#include "inputbox.h"
#include <QValidator>

InputBox::InputBox(QString textLabel, QWidget *parent) :
    QWidget(parent)
{
    vLayout = new QVBoxLayout(this);
    lbl = new QLabel(textLabel, this);
    lineEdit = new QLineEdit();

    vLayout->addWidget(lbl);
    vLayout->addWidget(lineEdit);

    lineEdit->setEnabled(true);
    lineEdit->setStyleSheet(" QLineEdit {border: 1px solid black; border-radius: 3px;\
                            background: #EFEBE7;}\
                            QLineEdit:enabled {border: 1px solid black; border-radius: 3px;\
                            background: #FFFFFF;}");
//    vLayout->setContentsMargins(2,2,2,0);
    vLayout->setContentsMargins(0,2,0,0);
//    vLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setAutoFillBackground(true);
}

QString InputBox::text()
{
    return lineEdit->text();
}

bool InputBox::setValidator(QString ValidType)
{
    if (ValidType == "int")
        validator = new QIntValidator();
    else if (ValidType == "double" || ValidType == "float")
        validator = new QDoubleValidator();
    else
        return false;

    lineEdit->setValidator(validator);
    return true;
}
