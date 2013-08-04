#ifndef SUPERPIXEL_H
#define SUPERPIXEL_H

#include <opencv2/highgui/highgui.hpp>
#include <bitset>

#include "OpenCVGraphSegmentation/Segmentation.h"

class SuperPixel
{
    uint64 maskFeature;
    double relHeightFeature;
    cv::Mat *HistSIFT[3];
    //int colorHist[100];
    void computeMaskFeature(vector<Pixel> &pixelList, int minX, int minY, int maxX, int maxY);
    void computeSiftFeature(cv::Mat &superPixelImg);
    void computeColorFeature(cv::Mat &superPixelImg);
public:
    SuperPixel(vector<Pixel> &list, cv::Mat &srcImg);
    ~SuperPixel();
    uint64 getMaskFeature();
    int getMaskDistance(uint64 otherMask);
};

#endif // SUPERPIXEL_H
