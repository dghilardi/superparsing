#include "tridimensionalobject.h"

TridimensionalObject::TridimensionalObject(int normalizationFactor, int pxllabel) : normFactor(normalizationFactor), label(pxllabel)
{
    //vertexList.push_back(QVector3D(-1.0, -1.0, 1.0));
    //vertexList.push_back(QVector3D( 1.0, -1.0,  1.0));
    //vertexList.push_back(QVector3D(-1.0,  1.0,  1.0));
    color = QVector4D((qrand()%255)/255.0, (qrand()%255)/255.0, (qrand()%255)/255.0, 1.0f);
}

void TridimensionalObject::addTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3){
    QVector3D vertexA(2*x1, -2*y1, -20*z1);
    QVector3D vertexB(2*x2, -2*y2, -20*z2);
    QVector3D vertexC(2*x3, -2*y3, -20*z3);
    QVector3D vertexAnorm(2*x1/normFactor-1, -2*y1/normFactor+1, -20*z1/normFactor);
    QVector3D vertexBnorm(2*x2/normFactor-1, -2*y2/normFactor+1, -20*z2/normFactor);
    QVector3D vertexCnorm(2*x3/normFactor-1, -2*y3/normFactor+1, -20*z3/normFactor);

    QVector3D normal;
    getNormal(vertexA, vertexB, vertexC, normal);
    string descriptor;
    getDescriptor(vertexA, normal, descriptor);
    //cout << "descr: " << descriptor << endl;

    if(!normal.isNull()){
        addVertex(vertexAnorm, normal);
        addVertex(vertexBnorm, normal);
        addVertex(vertexCnorm, normal);
        //vertexList.push_back(vertexAnorm);
        //vertexList.push_back(vertexBnorm);
        //vertexList.push_back(vertexCnorm);
    }
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

uint *TridimensionalObject::getIdxPtr(){
    return &(indicesList[0]);
}

int TridimensionalObject::vertexNumber(){
    return vertexList.size();
}

int TridimensionalObject::indicesNumber(){
    return indicesList.size();
}

TridimensionalObject::~TridimensionalObject(){

}

int TridimensionalObject::getLabel(){
    return label;
}

/**
 * @brief TridimensionalObject::getNormal Compute the normal of a triangle
 * @param vertexA
 * @param vertexB
 * @param vertexC
 * @param result
 */
void TridimensionalObject::getNormal(QVector3D &vertexA, QVector3D &vertexB, QVector3D &vertexC, QVector3D &result){
    QVector3D directionA, directionB;

    directionA = vertexA - vertexB;
    directionB = vertexB - vertexC;

    result = QVector3D(directionA.y()*directionB.z()-directionA.z()*directionB.y(),
                       directionA.z()*directionB.x()-directionA.x()*directionB.z(),
                       directionA.x()-directionB.y()-directionA.y()*directionB.x());
    result.normalize();



    result.setX((result.x()==0.0)?0.0:round(result.x()*10)/10);
    result.setY((result.y()==0.0)?0.0:round(result.y()*10)/10);
    result.setZ((result.z()==0.0)?0.0:round(result.z()*10)/10);
}

/**
 * @brief getDescriptor get the descriptor of a vertex
 * @param coordinates
 * @param normal
 * @param result
 */
void TridimensionalObject::getDescriptor(QVector3D &coordinates, QVector3D &normal, string &result){
    stringstream descr;
    descr << "(" << coordinates.x() << "," << coordinates.y() << "," << coordinates.z() << ")-(" << normal.x() << "," << normal.y() << "," << normal.z() << ")";
    result = descr.str();
}

void TridimensionalObject::addVertex(QVector3D &vertex, QVector3D &normal){
    string descriptor;
    getDescriptor(vertex, normal, descriptor);
    map<string, uint>::iterator it = idxMap.find(descriptor);
    if(it!=idxMap.end()){
        indicesList.push_back(it->second);
    }else{
        indicesList.push_back(vertexList.size());
        idxMap[descriptor] = vertexList.size();
        vertexList.push_back(vertex);
    }
}
