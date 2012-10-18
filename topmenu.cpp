#include "topmenu.h"

TopMenu::TopMenu(QWidget *parent) :
    QWidget(parent)
{
    setContentsMargins(2,2,2,2);

    QIcon::setThemeName("oxygen");
    topLayout = new QHBoxLayout;
    topLayout->setContentsMargins(0,0,0,0);
    topLayout->setSpacing(1);
    topLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
//    setStyleSheet("background-color: rgb(0, 0, 255); border:1px solid rgb(0, 0, 255);");

    addLineButton = new TopMenuButton;
//    QIcon ico;
//    ico.fromTheme("draw-line");
//    addLineButton->setIcon(ico);
    addLineButton->setIcon(QPixmap("/usr/share/icons/oxygen/16x16/actions/draw-line.png"));
    addLineButton->setToolTip("add line");

    addLayerButton = new TopMenuButton;
//    addLayerButton->setIcon(QIcon().fromTheme("image-loading"));
    addLayerButton->setIcon(QPixmap("/usr/share/icons/oxygen/16x16/status/image-loading.png"));
    addLayerButton->setToolTip("overlay an image");

    topLayout->addWidget(addLineButton);
    topLayout->addWidget(addLayerButton);
    setLayout(topLayout);
}
