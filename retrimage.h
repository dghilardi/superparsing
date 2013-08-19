#ifndef RETRIMAGE_H
#define RETRIMAGE_H

#include <list>
#include <set>

#include "debugHeader.h"

#include "image.h"
#include "geolabel.h"
#include "OpenCVGraphSegmentation/Segmentation.h"
#include "superpixel.h"
#include "neighbourstat.h"

class RetrImage
{
    Image image;
    GeoLabel labeledImg;
    vector<SuperPixel *> superPixelList;
    void buildSuperPixels();
public:
    RetrImage(string imgName);
    ~RetrImage();
    void show();
    vector<SuperPixel *> *getSuperPixels();
    string matchLabel(int id);
    void updateNeighbourStatistics(NeighbourStat &statistics);
};

#endif // RETRIMAGE_H
