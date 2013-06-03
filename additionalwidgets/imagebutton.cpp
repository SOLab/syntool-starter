#include "imagebutton.h"

ImageButton::ImageButton(QString image_name, QString label, QWidget *parent) :
    QPushButton(parent)
{
    m_layout = new QVBoxLayout(this);
    m_layout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    image_lbl = new QLabel(this);
//    if (!image_name.isNull())
//        image_name = "";

//    bool icon_flag = true;

    QIcon icon;
    icon.addFile(image_name);
    QPixmap pm2 = icon.pixmap(32);
//    QImageReader ir(image_name);
//    ir.setScaledSize(QSize(32, 32));
//    QImage img = ir.read();
//    QPixmap pm2 = QPixmap().fromImage(img);

    pm2 = pm2.scaled(QSize(32,32), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    image_lbl->setPixmap(pm2);
    image_lbl->setMinimumHeight(32);
    image_lbl->setAlignment(Qt::AlignHCenter);

    // add transparency
    image_lbl->setAttribute(Qt::WA_NoSystemBackground);

    m_layout->addWidget(image_lbl);
    setFlat(true);

//    if (!label.isNull())
//        label = "";
    QLabel* lbl = new QLabel(label, this);
    lbl->setWordWrap(true);
    lbl->setFixedHeight(30);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    lbl->resize(lbl->sizeHint());
    lbl->setStyleSheet("QLabel {border: None;}");
    // add transparency
    lbl->setAttribute(Qt::WA_NoSystemBackground);

    m_layout->addWidget(lbl);
    m_layout->setContentsMargins(0,8,0,0);
//    m_layout->setSpacing(28);

    setFixedSize(96,80);
//    setObjectName(name)

    setStyleSheet("QPushButton:flat {border: None;}"
                       "QPushButton:hover:pressed {border: none;"
                       "background-color: rgb(218,218,218);"
                       "border-radius: 7px;}"
                       "QPushButton:hover:!pressed{background-color: "
                       "rgb(230,230,230); border-radius: 7px;}");
    updateGeometry();
}
