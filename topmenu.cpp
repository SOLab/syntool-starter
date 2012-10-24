#include "topmenu.h"
#include "QDebug"

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
    addLineButton->setIcon(QIcon().fromTheme("draw-line"));
//    addLineButton->setIcon(QPixmap("/usr/share/icons/oxygen/16x16/actions/draw-line.png"));
    addLineButton->setToolTip("Add line");

    addLayerButton = new TopMenuButton;
    addLayerButton->setIcon(QIcon().fromTheme("image-loading"));
//    addLayerButton->setIcon(QPixmap("/usr/share/icons/oxygen/16x16/status/image-loading.png"));
    addLayerButton->setToolTip("Overlay an image");

    TimeLineButton = new TopMenuButton;
    TimeLineButton->setIcon(QIcon().fromTheme("view-time-schedule"));
    TimeLineButton->setToolTip("TimeLine");
    connect(TimeLineButton, SIGNAL(clicked()), parent, SLOT(showTimeLine()));

    PythonScriptButton = new TopMenuButton;
//    PythonScriptButton->setIcon(QPixmap("/usr/share/icons/oxygen/16x16/mimetypes/text-x-python.png"));
    PythonScriptButton->setIcon(QIcon().fromTheme("text-x-python"));
    qDebug() << PythonScriptButton->icon();
    PythonScriptButton->setToolTip("PythonQt Script");

    leftLayout = new QHBoxLayout;
    rightLayout = new QHBoxLayout;

    leftLayout->addWidget(addLineButton);
    leftLayout->addWidget(addLayerButton);
    rightLayout->addWidget(TimeLineButton, 0, Qt::AlignRight);
    rightLayout->addWidget(PythonScriptButton, 0, Qt::AlignRight);

    topLayout->addLayout(leftLayout, 10);
    topLayout->setAlignment(leftLayout, Qt::AlignLeft);
    topLayout->addLayout(rightLayout, 0);
    topLayout->setAlignment(rightLayout, Qt::AlignRight);
    setLayout(topLayout);
}
