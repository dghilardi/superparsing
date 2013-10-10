#ifndef TRIDIMENSIONALOBJECT_H
#define TRIDIMENSIONALOBJECT_H

#include <QVector3D>
#include <QVector4D>
#include <vector>

using namespace std;
class TridimensionalObject
{
    vector<QVector3D> vertexList;
    QVector4D color;
    int normFactor;
    int label;
public:
    TridimensionalObject(int normalizationFactor, int pxllabel);
    ~TridimensionalObject();

    QVector3D *getPtr();
    QVector4D *getColorPtr();
    int vertexNumber();
    void setColor(float r, float g, float b);
    void addTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
    int getLabel();
};

#endif // TRIDIMENSIONALOBJECT_H
