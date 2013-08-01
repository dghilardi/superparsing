#ifndef QUERYIMAGE_H
#define QUERYIMAGE_H

#include <vector>

#include "image.h"
#include "OpenCVGraphSegmentation/Segmentation.h"

using namespace std;

class QueryImage
{
    Image image;
    vector<vector<Pixel> > superPixels;
public:
    QueryImage(string filename);
};

#endif // QUERYIMAGE_H
