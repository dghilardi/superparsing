#include "tridimensionalobject.h"

TridimensionalObject::TridimensionalObject()
{
    color = QVector4D((qrand()%255)/255.0, (qrand()%255)/255.0, (qrand()%255)/255.0, 1.0f);
}

void TridimensionalObject::addTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
    vertexList.push_back(QVector3D(x1, y1, z1));
    vertexList.push_back(QVector3D(x2, y2, z2));
    vertexList.push_back(QVector3D(x3, y3, z3));
}

void TridimensionalObject::setColor(float r, float g, float b){
    color = QVector4D(r, g, b, 1.0f);
}

QVector4D *TridimensionalObject::getColorPtr(){
    return &color;
}

QVector3D *TridimensionalObject::getPtr(){
    return &(vertexList[0]);
}

int TridimensionalObject::vertexNumber(){
    return vertexList.size();
}

TridimensionalObject::~TridimensionalObject(){

}
