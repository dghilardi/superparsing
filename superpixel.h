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
#include "quantizedsift.h"
#include <cmath>

using namespace std;
class SuperPixel
{
    cv::Point topLeftCorner;
    uint64 maskFeature;
    double relHeightFeature;
    double weight;
    cv::Mat *colorHist[3];
    float siftDescriptor[QUANTIZATION_SIZE];
    cv::Mat superPixelImg;
    cv::Mat mask;
    vector<Pixel> pixelCoordList;
    set<SuperPixel *> adiacentsSet;
    int label;
    void computeMaskFeature(vector<Pixel> &pixelList, int minX, int minY, int maxX, int maxY);    
    void computeColorFeature(cv::Mat &superPixelImg);
    void computeSIFTDescriptor(QuantizedSift *quantization);
public:
    SuperPixel(vector<Pixel> &list, cv::Mat &srcImg, bool computeSIFT=true);
    SuperPixel(cv::Mat &img, cv::Mat &spMask, bool computeSIFT);
    ~SuperPixel();
    void computeSiftFeature(cv::Mat &descriptor, bool computeDense=true);
    int getLabel();
    double getWeight();
    void setLabel(int newLabel);
    //uint64 getMaskFeature();
    //int getMaskDistance(uint64 otherMask);
    int getMaskDistance(SuperPixel &otherSP);
    float getRelHeightDistance(SuperPixel &otherSP);
    double getSiftDistance(SuperPixel &otherSP);
    double getColorDistance(SuperPixel &otherSP);
    cv::Mat *getMask();
    cv::Point getTopLeftCorner();
    void show();
    void printToMat(cv::Mat &result);
    void appendAdiacent(SuperPixel *toAppend);
    const set<SuperPixel *> *getAdiacents();
    const vector<Pixel> *getCoordList();
    static void computeAdiacents(vector<SuperPixel *> &spList, int height, int width);
    static void computeWeight(vector<SuperPixel *> &spList);
};

#endif // SUPERPIXEL_H
