#ifndef TRIDIMENSIONALVOXEL_H
#define TRIDIMENSIONALVOXEL_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "poly2tri/poly2tri.h"
#define POLY2TRI

#include "Utils/visualutils.h"
#include "tridimensionalobject.h"
#include "../superpixel.h"

using namespace std;

class Segment{
    p2t::Point *src;
    p2t::Point *dst;
public:
    Segment(p2t::Point *pointA, p2t::Point *pointB){
        src = new p2t::Point();
        dst = new p2t::Point();
        if(pointA->x < pointB->x || (pointA->x == pointB->x && pointA->y <= pointB->y)){
            *src = *pointA;
            *dst = *pointB;
        }else{
            *src = *pointB;
            *dst = *pointA;
        }
    }
    ~Segment(){
        delete src;
        delete dst;
    }

    p2t::Point *getSrc(){ return src; }
    p2t::Point *getDst(){ return dst; }
    string str(){
        stringstream descr;
        descr <<"["<< src->x << " - " << src->y << "] -> [" << dst->x << " - " << dst->y << "]";
        return descr.str();
    }
};

class TridimensionalVoxel
{
private:
    TridimensionalObject *triangles;
    typedef cv::Vec<float, 9> Tri;

    void getTrianglesList(SuperPixel *prevSP, SuperPixel *nextSP, TridimensionalObject &result, int frameNumber, map<string, Segment*> &prevContour);
    void extrudeOnePxl(vector<vector<p2t::Point *> > *contours, TridimensionalObject &triangles, int frameNumber, map<string, Segment*> &prevContour);
    bool isVertex(int x, int y, cv::Mat *mask);

    void xorMatrices(cv::Mat *mat1, cv::Mat *mat2, cv::Point mat1Offset, cv::Point mat2Offset, cv::Mat *result);
    void cleanTriangles(vector<cv::Vec6f> &allTriangles, vector<cv::Vec6f> &result, cv::Mat &mask);
    void drawTriangles(vector<Tri> &triangles, cv::Mat *mask);
    void triangulateFace(TridimensionalObject &result, cv::Mat *xorMatrix, int frameNumber, vector<vector<p2t::Point *> > &polylines);
    string type2str(int type);
public:
    TridimensionalVoxel(vector<SuperPixel *> *spList, int firstFrameIdx, cv::Size imgDim);
    ~TridimensionalVoxel();
    TridimensionalObject *getObject();
};

#endif // TRIDIMENSIONALVOXEL_H
