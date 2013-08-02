#ifndef SUPERPIXEL_H
#define SUPERPIXEL_H

#include <opencv2/highgui/highgui.hpp>
#include <bitset>

#include "OpenCVGraphSegmentation/Segmentation.h"

class SuperPixel
{
    cv::Mat *superPixel;
    cv::Mat *mask;
    uint64 maskFeature;
    double relHeightFeature;
    void computeMaskFeature(vector<Pixel> &pixelList, int minX, int minY, int maxX, int maxY);
public:
    SuperPixel(vector<Pixel> &list, cv::Mat &srcImg);
    ~SuperPixel();
    uint64 getMaskFeature();
    int getMaskDistance(uint64 otherMask);
};

#endif // SUPERPIXEL_H
