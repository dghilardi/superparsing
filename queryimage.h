#ifndef QUERYIMAGE_H
#define QUERYIMAGE_H

#include <vector>

#include "debugHeader.h"

#include "image.h"
#include "superpixel.h"
#include "geolabel.h"
#include "OpenCVGraphSegmentation/Segmentation.h"

using namespace std;

class QueryImage
{
    Image image;
    string imgName;
    vector<SuperPixel *> superPixelList;
    void buildObtainedMat(cv::Mat &result);
public:
    QueryImage(string filename);
    ~QueryImage();

    vector<SuperPixel *> *getSuperPixels();
    void showLabeling();
    void showSrc();
    double checkResults();
};

#endif // QUERYIMAGE_H
