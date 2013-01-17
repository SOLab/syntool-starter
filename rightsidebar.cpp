#include "rightsidebar.h"

RightSidebar::RightSidebar()
{
    setMinimumWidth(100);
    setStyleSheet("QToolBox::tab {background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                  stop: 0 #EFEBE7, stop: 0.4 #DDDDDD,\
                  stop: 0.5 #D8D8D8, stop: 1.0 #D3D3D3);\
                  border-radius: 5px;\
                  color: black; border: 1px solid darkgrey;} \
//                  QToolBox { border: 1px solid black;} \
                  ");
}
