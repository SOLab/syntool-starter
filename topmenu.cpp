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
    moveButton->setCheckable(true);
    connect(moveButton, &QPushButton::clicked, this, &TopMenu::setCheckedButton);
    connect(moveButton, &QPushButton::clicked, this, &TopMenu::setMoveAction);

    addLineButton = new TopMenuButton;
    addLineButton->setIcon(QIcon(":/icons/line.png"));
    addLineButton->setIconSize(QSize(16,16));
    addLineButton->setToolTip(tr("Add line"));
    addLineButton->setCheckable(true);
    connect(addLineButton, &QPushButton::clicked, this, &TopMenu::setCheckedButton);
    connect(addLineButton, &QPushButton::clicked, this, &TopMenu::addLineAction);

    addRectButton = new TopMenuButton;
    addRectButton->setIcon(QIcon(":/icons/rectangle.png"));
    addRectButton->setIconSize(QSize(16,16));
    addRectButton->setToolTip(tr("Add rectangle"));
    addRectButton->setCheckable(true);
    connect(addRectButton, &QPushButton::clicked, this, &TopMenu::setCheckedButton);
    connect(addRectButton, &QPushButton::clicked, this, &TopMenu::addRectAction);

    addTagButton = new TopMenuButton;
    addTagButton->setIcon(QIcon(":/icons/tag.png"));
    addTagButton->setIconSize(QSize(16,16));
    addTagButton->setToolTip(tr("Add tag"));
    addTagButton->setCheckable(true);
    connect(addTagButton, &QPushButton::clicked, this, &TopMenu::setCheckedButton);
    connect(addTagButton, &QPushButton::clicked, this, &TopMenu::addTagAction);

    addPinButton = new TopMenuButton;
    addPinButton->setIcon(QIcon(":/icons/pin.png"));
    addPinButton->setIconSize(QSize(16,16));
    addPinButton->setToolTip(tr("Add pin"));
    addPinButton->setCheckable(true);
    connect(addPinButton, &QPushButton::clicked, this, &TopMenu::setCheckedButton);
    connect(addPinButton, &QPushButton::clicked, this, &TopMenu::addPinAction);

    showCoordsButton = new TopMenuButton;
    showCoordsButton->setIcon(QIcon(":/icons/xy.png"));
    showCoordsButton->setToolTip(tr("Show coordinates"));
    showCoordsButton->setCheckable(true);
    connect(showCoordsButton, &QPushButton::clicked, this, &TopMenu::showCoordsAction);

    addGridButton = new TopMenuButton;
    addGridButton->setIcon(QIcon(":/icons/grid.png"));
    addGridButton->setToolTip(tr("Show grid"));
    addGridButton->setCheckable(true);
    connect(addGridButton, &QPushButton::clicked, this, &TopMenu::showGridAction);

    addShowButton = new TopMenuButton;
    addShowButton->setIcon(QIcon(":/icons/show.png"));
    addShowButton->setToolTip(tr("Show/Hide all layers"));
    addShowButton->setCheckable(true);
    connect(addShowButton, &QPushButton::clicked, this, &TopMenu::hideAllAction);

    addPictureButton = new TopMenuButton;
    addPictureButton->setIcon(QIcon(":/icons/picture.png"));
    addPictureButton->setToolTip(tr("Overlay an image"));
    connect(addPictureButton, &QPushButton::clicked, this, &TopMenu::addPictureLayerAction);

    // create right side

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

    QFrame* VLine = new QFrame();
    VLine->setFrameShape(QFrame::VLine);
    VLine->setFrameShadow(QFrame::Sunken);
    leftLayout->addWidget(VLine);

    leftLayout->addWidget(showCoordsButton);
    leftLayout->addWidget(addGridButton);
    leftLayout->addWidget(addShowButton);
    leftLayout->addWidget(addPictureButton);

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

void TopMenu::unSelectAll()
{
    moveButton->setChecked(false);
    addLineButton->setChecked(false);
    addRectButton->setChecked(false);
    addTagButton->setChecked(false);
    addPinButton->setChecked(false);
}

void TopMenu::setCheckedButton(bool value)
{
    if (value)
    {
        unSelectAll();
        TopMenuButton* _sender = qobject_cast<TopMenuButton*>(sender());
        _sender->setChecked(true);
    }
    else
    {
        unSelectAll();
        moveButton->setChecked(true);
    }
}
