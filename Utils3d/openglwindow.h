#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QMainWindow>
#include "openglwidget.h"
#include "tridimensionalobject.h"
namespace Ui {
class OpenGLWindow;
}

class OpenGLWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OpenGLWindow(vector<GeometryEngine *> *obj, QWidget *parent = 0);
    ~OpenGLWindow();

private:
    Ui::OpenGLWindow *ui;
};

#endif // OPENGLWINDOW_H
