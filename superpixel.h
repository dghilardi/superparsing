#ifndef SUPERPIXEL_H
#define SUPERPIXEL_H

#include "debugHeader.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <bitset>
#include <set>
#include "OpenCVGraphSegmentation/Segmentation.h"
#include <cmath>

using namespace std;
class SuperPixel
{
    uint64 maskFeature;
    double relHeightFeature;
    double weight;
    cv::Mat *colorHist[3];
    //cv::Mat siftHist;
    cv::Mat superPixelImg;
    vector<Pixel> pixelCoordList;
    set<SuperPixel *> adiacentsSet;
    int label;
    void computeMaskFeature(vector<Pixel> &pixelList, int minX, int minY, int maxX, int maxY);    
    void computeColorFeature(cv::Mat &superPixelImg);
public:
    SuperPixel(vector<Pixel> &list, cv::Mat &srcImg);
    ~SuperPixel();
    void computeSiftFeature(cv::Mat &descriptor);
    int getLabel();
    double getWeight();
    void setLabel(int newLabel);
    //uint64 getMaskFeature();
    //int getMaskDistance(uint64 otherMask);
    int getMaskDistance(SuperPixel &otherSP);
    float getRelHeightDistance(SuperPixel &otherSP);
    double getSiftDistance(SuperPixel &otherSP);
    double getColorDistance(SuperPixel &otherSP);
    void show();
    void printToMat(cv::Mat &result);
    void appendAdiacent(SuperPixel *toAppend);
    const set<SuperPixel *> *getAdiacents();
    const vector<Pixel> *getCoordList();
    static void computeAdiacents(vector<SuperPixel *> &spList, int height, int width);
    static void computeWeight(vector<SuperPixel *> &spList);
};

#endif // SUPERPIXEL_H
