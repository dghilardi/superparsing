#ifndef QUERYIMAGE_H
#define QUERYIMAGE_H

#include <vector>



#include "image.h"
#include "superpixel.h"
#include "geolabel.h"
#include "Utils/visualutils.h"
#include "OpenCVGraphSegmentation/Segmentation.h"

using namespace std;

class QueryImage
{
    Image image;
    string imgName;
    vector<SuperPixel *> superPixelList;
    void buildObtainedMat(cv::Mat &result, set<int> *usedLabels=NULL);
public:
    QueryImage(string filename, bool computeSIFT=true);
    ~QueryImage();

    vector<SuperPixel *> *getSuperPixels();
    void showLabeling(string title="LABELED IMAGE");
    void showOriginalLabeling();
    void showSrc();
    double checkResults();
};

#endif // QUERYIMAGE_H
