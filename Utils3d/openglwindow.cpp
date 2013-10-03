#include "openglwindow.h"
#include "ui_openglwindow.h"

OpenGLWindow::OpenGLWindow(vector<GeometryEngine *> *obj, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OpenGLWindow)
{
    ui->setupUi(this);
}

OpenGLWindow::~OpenGLWindow()
{
    delete ui;
}
