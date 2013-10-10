#include "openglwindow.h"
#include "ui_openglwindow.h"

#include "Utils3d/tridimensionalobject.h"
#include "Utils3d/geometryengine.h"
OpenGLWindow::OpenGLWindow(vector<GeometryEngine *> *obj, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OpenGLWindow),
    objectsList(obj)
{
    ui->setupUi(this);
    std::set<int> usedLabels;
    for(uint i=0; i<obj->size(); ++i){
        GeometryEngine *geom = obj->at(i);
        TridimensionalObject *objData = geom->getObject();
        usedLabels.insert(objData->getLabel());
        ui->widget->addObj(geom);
    }

    connect(ui->zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(modifyZoom(int)));
    connect(ui->speedSlider, SIGNAL(valueChanged(int)), this, SLOT(modifySpeed(int)));
    for(std::set<int>::iterator it=usedLabels.begin(); it!=usedLabels.end(); ++it){
        QCheckBox *checkBox = new QCheckBox(ui->centralwidget);
        checkBox->setObjectName(QString::number(*it)+"_checkbox");
        checkBoxMap[checkBox->objectName()] = *it;
        checkBox->setText(QString(GeoLabel::getLabel(*it).c_str()));
        checkBox->setChecked(true);
        connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(changeCheckboxState(bool)));
        ui->verticalLayout->addWidget(checkBox);
    }
}

OpenGLWindow::~OpenGLWindow()
{
    delete ui;
}

void OpenGLWindow::changeCheckboxState(bool newState){
    QCheckBox *sender = qobject_cast<QCheckBox *>(QObject::sender());
    int label = checkBoxMap[sender->objectName()];
    for(int i=0; i<objectsList->size(); ++i){
        if(objectsList->at(i)->getObject()->getLabel()==label) objectsList->at(i)->setDraw(newState);
    }
}

void OpenGLWindow::modifyZoom(int value){

    ui->widget->setZoom(-(value/50.0-1)*4);
}

void OpenGLWindow::modifySpeed(int value){

    ui->widget->setSpeed((value/50.0-1)*3);
}
