#include "topmenu.h"
#include "QSplitter"

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

    moveButton = new TopMenuButton;
    moveButton->setIcon(QIcon(":/icons/move.png"));
    moveButton->setIconSize(QSize(16,16));
    moveButton->setToolTip(tr("Move"));

    addLineButton = new TopMenuButton;
    addLineButton->setIcon(QIcon(":/icons/line.png"));
    addLineButton->setIconSize(QSize(16,16));
    addLineButton->setToolTip(tr("Add line"));

    addRectButton = new TopMenuButton;
    addRectButton->setIcon(QIcon(":/icons/rectangle.png"));
    addRectButton->setIconSize(QSize(16,16));
    addRectButton->setToolTip(tr("Add rectangle"));

    addTagButton = new TopMenuButton;
    addTagButton->setIcon(QIcon(":/icons/tag.png"));
    addTagButton->setIconSize(QSize(16,16));
    addTagButton->setToolTip(tr("Add tag"));

    addPinButton = new TopMenuButton;
    addPinButton->setIcon(QIcon(":/icons/pin.png"));
    addPinButton->setIconSize(QSize(16,16));
    addPinButton->setToolTip(tr("Add pin"));

    addLayerButton = new TopMenuButton;
    addLayerButton->setIcon(QIcon(":/icons/layers.png"));
    addLayerButton->setToolTip(tr("Add layer"));

    addGridButton = new TopMenuButton;
    addGridButton->setIcon(QIcon(":/icons/grid.png"));
    addGridButton->setToolTip(tr("Show grid"));

    addPictureButton = new TopMenuButton;
    addPictureButton->setIcon(QIcon(":/icons/picture.png"));
    addPictureButton->setToolTip(tr("Overlay an image"));

    addShowButton = new TopMenuButton;
    addShowButton->setIcon(QIcon(":/icons/show.png"));
    addShowButton->setToolTip(tr("Show/Hide all layers"));

    TimeLineButton = new TopMenuButton;
    TimeLineButton->setIcon(QIcon(":/icons/time_line.png"));
    TimeLineButton->setIconSize(QSize(16,16));
    TimeLineButton->setToolTip(tr("TimeLine"));
    connect(TimeLineButton, SIGNAL(clicked()), parent, SLOT(showTimeLine()));

    PythonScriptButton = new TopMenuButton;
    PythonScriptButton->setIcon(QIcon(":/icons/script.png"));
    PythonScriptButton->setToolTip(tr("PythonQt Script"));

    leftLayout = new QHBoxLayout;
    rightLayout = new QHBoxLayout;

    leftLayout->addWidget(moveButton);
    leftLayout->addWidget(addLineButton);
    leftLayout->addWidget(addRectButton);
    leftLayout->addWidget(addTagButton);
    leftLayout->addWidget(addPinButton);
    leftLayout->addWidget(addLayerButton);

    QFrame* VLine = new QFrame();
    VLine->setFrameShape(QFrame::VLine);
    VLine->setFrameShadow(QFrame::Sunken);
    leftLayout->addWidget(VLine);

    leftLayout->addWidget(addGridButton);
    leftLayout->addWidget(addPictureButton);
    leftLayout->addWidget(addShowButton);

    QFrame* VLine2 = new QFrame();
    VLine2->setFrameShape(QFrame::VLine);
    VLine2->setFrameShadow(QFrame::Sunken);
    rightLayout->addWidget(VLine2);
    rightLayout->addWidget(TimeLineButton, 0, Qt::AlignRight);
    rightLayout->addWidget(PythonScriptButton, 0, Qt::AlignRight);

    topLayout->addLayout(leftLayout, 10);
    topLayout->setAlignment(leftLayout, Qt::AlignLeft);
    topLayout->addLayout(rightLayout, 0);
    topLayout->setAlignment(rightLayout, Qt::AlignRight);
    setLayout(topLayout);
}
