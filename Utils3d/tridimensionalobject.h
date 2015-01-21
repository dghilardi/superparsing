#ifndef TRIDIMENSIONALOBJECT_H
#define TRIDIMENSIONALOBJECT_H
#include <qt4/Qt/qvector3d.h>
#include <qt4/Qt/qvector4d.h>
//#include <QVector3D>
//#include <QVector4D>
#include <vector>
#include <map>
#include <cassert>
#include <sstream>
#include <iostream>
#include <cmath>
using namespace std;
class TridimensionalObject
{
    vector<QVector3D> vertexList;
    vector<unsigned int> indicesList;
    map<string, uint> idxMap;
    QVector4D color;
    int normFactor;
    int label;

    void getNormal(QVector3D &vertexA, QVector3D &vertexB, QVector3D &vertexC, QVector3D &result);
    void getDescriptor(QVector3D &coordinates, QVector3D &normal, string &result);
    void addVertex(QVector3D &vertex, QVector3D &normal);
public:
    TridimensionalObject(int normalizationFactor, int pxllabel);
    ~TridimensionalObject();

    QVector3D *getPtr();
    uint *getIdxPtr();
    QVector4D *getColorPtr();
    int vertexNumber();
    int indicesNumber();
    void setColor(float r, float g, float b);
    void addTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
    int getLabel();
};

#endif // TRIDIMENSIONALOBJECT_H
