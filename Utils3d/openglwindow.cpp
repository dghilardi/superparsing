#include "openglwindow.h"
#include "ui_openglwindow.h"

#include "Utils3d/tridimensionalobject.h"
#include "Utils3d/geometryengine.h"
OpenGLWindow::OpenGLWindow(vector<GeometryEngine *> *obj, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OpenGLWindow)
{
    ui->setupUi(this);
    for(uint i=0; i<obj->size(); ++i)
            ui->widget->addObj(obj->at(i));
}

OpenGLWindow::~OpenGLWindow()
{
    delete ui;
}
