#ifndef SUPERPIXEL_H
#define SUPERPIXEL_H

#include <opencv2/highgui/highgui.hpp>
#include <bitset>

#include "OpenCVGraphSegmentation/Segmentation.h"

using namespace std;
class SuperPixel
{
    uint64 maskFeature;
    double relHeightFeature;
    cv::Mat *colorHist[3];
    cv::Mat siftHist;
    cv::Mat superPixelImg;
    int label;
    void computeMaskFeature(vector<Pixel> &pixelList, int minX, int minY, int maxX, int maxY);
    void computeSiftFeature(cv::Mat &superPixelImg);
    void computeColorFeature(cv::Mat &superPixelImg);
public:
    SuperPixel(vector<Pixel> &list, cv::Mat &srcImg);
    ~SuperPixel();
    int getLabel();
    void setLabel(int newLabel);
    //uint64 getMaskFeature();
    //int getMaskDistance(uint64 otherMask);
    int getMaskDistance(SuperPixel &otherSP);
    float getRelHeightDistance(SuperPixel &otherSP);
    double getSiftDistance(SuperPixel &otherSP);
    double getColorDistance(SuperPixel &otherSP);
    void show();
};

#endif // SUPERPIXEL_H
