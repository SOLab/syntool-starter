#include "satinlogin.h"

SatinLoginWidget::SatinLoginWidget(QWidget *parent) :
    QWidget()
{
    gLayout = new QGridLayout(this);
    gLayout->setColumnStretch(0,1);
    gLayout->setColumnStretch(1,5);

    loginLbl = new QLabel(tr("Email:"), this);
    loginEdit = new QLineEdit(this);

    gLayout->addWidget(loginLbl, 0,0,1,1,Qt::AlignRight);
    gLayout->addWidget(loginEdit, 0,1);

    passwdLbl = new QLabel(tr("Password:"), this);
    passwdEdit = new QLineEdit(this);
    passwdEdit->setEchoMode(QLineEdit::Password);

    gLayout->addWidget(passwdLbl, 1,0,1,1,Qt::AlignRight);
    gLayout->addWidget(passwdEdit, 1,1);

    signInButton = new QPushButton(tr("Sing in"), this);
    connect(signInButton, &QPushButton::clicked, this, &SatinLoginWidget::singInSlot);

    cancelButton = new QPushButton(tr("Cancel"), this);
    connect(cancelButton, &QPushButton::clicked, this, &SatinLoginWidget::close);

    gLayout->addWidget(signInButton, 2,0,1,1,Qt::AlignCenter);
    gLayout->addWidget(cancelButton, 2,1,1,1,Qt::AlignCenter);

    setWindowTitle(tr("Satin sing in"));
    setFixedSize(256, 128);

    registerButton = new QLabel(this);
    registerButton->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    registerButton->setOpenExternalLinks(true);
    registerButton->setText("<a href=\"https://satin.rshu.ru/Auth/Register\">Register\</a>");

    recoverButton = new QLabel(this);
    recoverButton->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
    recoverButton->setOpenExternalLinks(true);
    recoverButton->setText("<a href=\"https://satin.rshu.ru/Auth/RestorePassword\">Recover password</a>");

    gLayout->addWidget(registerButton, 3,0,1,1,Qt::AlignCenter);
    gLayout->addWidget(recoverButton, 3,1,1,1,Qt::AlignCenter);

    hide();
    show();
}

void SatinLoginWidget::singInSlot()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QUrl url("https://satin.rshu.ru/Auth");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("Email", loginEdit->text());
    params.addQueryItem("Password", passwdEdit->text());
    // etc

    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));

    manager->post(request, params.query(QUrl::FullyEncoded).toUtf8());
    qCritical() << loginEdit->text();
    qCritical() << passwdEdit->text();
}

void SatinLoginWidget::replyFinished(QNetworkReply *reply)
{
    qCritical() << 11111111 << reply->readAll();
    qCritical() << 22222222 << reply->error();
}
