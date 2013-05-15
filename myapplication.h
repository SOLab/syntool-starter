#ifndef KEYSINTERCEPTION_H
#define KEYSINTERCEPTION_H

#include <QtGui>
#include <QtCore>
#include "3d/earthview.h"
#include "mainwidget.h"
#include <QKeyEvent>
#include <QApplication>

class myApplication : public QApplication
{
public:
    myApplication(int argc ,char** argv):
    QApplication( argc, argv)
    {
        _view = NULL;
    }
    ~myApplication()
    {}

    bool notify(QObject* object,QEvent* event)
    {

      //If this is keypress event , do not send it to the receiver
      if(event->type() == QEvent::KeyPress)
      {
          QKeyEvent * key = (QKeyEvent *) event;
          if (_view)
          {
              switch (key->key())
              {
                  case Qt::Key_Left:
                  case Qt::Key_Right:
                  case Qt::Key_Up:
                  case Qt::Key_Down:
                  case Qt::Key_Plus:
                  case Qt::Key_Minus:
                  {
                      _view->keyPress(key);
                  }
                  break;
              }
          }

        QApplication::notify(object, event);
        return true;
      }
      //else do the regular processing
      else
      {
          return QApplication::notify(object, event);
      }

    }

void set_view(EarthView *view)
    {
        _view = view;
    }

protected:
    EarthView* _view;
};

#endif // KEYSINTERCEPTION_H
