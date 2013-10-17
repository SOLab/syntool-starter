#ifndef SATINLOGIN_H
#define SATINLOGIN_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QNetworkReply>

#include <QDebug>

class SatinLoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SatinLoginWidget(QWidget *parent = 0);
    QGridLayout* gLayout;
    QLabel* loginLbl;
    QLineEdit* loginEdit;
    QLabel* passwdLbl;
    QLineEdit* passwdEdit;

    QPushButton* signInButton;
    QPushButton* cancelButton;
    QLabel*      registerButton;
    QLabel*      recoverButton;

signals:

public slots:
    void singInSlot();
    void replyFinished(QNetworkReply *reply);

};

#endif // SATINLOGIN_H
