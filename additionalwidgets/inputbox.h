#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRegExp>

class InputBox : public QWidget
{
    Q_OBJECT
public:
    explicit InputBox(QString textLabel, QWidget *parent = 0);
    QString text();
    bool setValidator(QString ValidType);

protected:
    QVBoxLayout* vLayout;
    QLabel* lbl;
    QLineEdit* lineEdit;
    QValidator *validator;

signals:
    
public slots:
    
};

#endif // INPUTBOX_H
