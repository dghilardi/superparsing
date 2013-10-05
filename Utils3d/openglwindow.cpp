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
        /*if(obj->at(i)->getDimension()>1000000)*/ui->widget->addObj(obj->at(i));
    //int GeometryEngine::getDimension()
}

OpenGLWindow::~OpenGLWindow()
{
    delete ui;
}
