#ifndef QUERYIMAGE_H
#define QUERYIMAGE_H

#include <vector>

#include "image.h"
#include "superpixel.h"
#include "OpenCVGraphSegmentation/Segmentation.h"

using namespace std;

class QueryImage
{
    Image image;
    vector<SuperPixel *> superPixelList;
public:
    QueryImage(string filename);
    ~QueryImage();

    vector<SuperPixel *> *getSuperPixels();
    void showLabeling();
    void showSrc();
};

#endif // QUERYIMAGE_H
