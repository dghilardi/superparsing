#include "tridimensionalobject.h"

TridimensionalObject::TridimensionalObject(int normalizationFactor) : normFactor(normalizationFactor)
{
    //vertexList.push_back(QVector3D(-1.0, -1.0, 1.0));
    //vertexList.push_back(QVector3D( 1.0, -1.0,  1.0));
    //vertexList.push_back(QVector3D(-1.0,  1.0,  1.0));
    color = QVector4D((qrand()%255)/255.0, (qrand()%255)/255.0, (qrand()%255)/255.0, 1.0f);
}

void TridimensionalObject::addTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
    vertexList.push_back(QVector3D(2*x1/normFactor-1, -2*y1/normFactor+1, -10*z1/normFactor));
    vertexList.push_back(QVector3D(2*x2/normFactor-1, -2*y2/normFactor+1, -10*z2/normFactor));
    vertexList.push_back(QVector3D(2*x3/normFactor-1, -2*y3/normFactor+1, -10*z3/normFactor));
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
