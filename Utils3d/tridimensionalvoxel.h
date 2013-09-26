#ifndef TRIDIMENSIONALVOXEL_H
#define TRIDIMENSIONALVOXEL_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <iostream>

#include "poly2tri/geometry.h"
#include "poly2tri/defs.h"
#define POLY2TRI

#include "../superpixel.h"
#include "../debugHeader.h"
using namespace std;
class TridimensionalVoxel
{
private:
    void getTrianglesList(SuperPixel *prevSP, SuperPixel *nextSP, vector<cv::Vec6f> &result);
    bool isVertex(int x, int y, cv::Mat *mask);

    void xorMatrices(cv::Mat *mat1, cv::Mat *mat2, cv::Point mat1Offset, cv::Point mat2Offset, cv::Mat *result);
    void cleanTriangles(vector<cv::Vec6f> &allTriangles, vector<cv::Vec6f> &result, cv::Mat &mask);
    void drawTriangles(vector<cv::Vec6f> &triangles, cv::Mat *mask);
public:
    TridimensionalVoxel(vector<SuperPixel *> &spList);
};

#endif // TRIDIMENSIONALVOXEL_H
