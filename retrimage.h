#ifndef RETRIMAGE_H
#define RETRIMAGE_H

#include <list>

#include "debugHeader.h"

#include "image.h"
#include "geolabel.h"
#include "OpenCVGraphSegmentation/Segmentation.h"
#include "superpixel.h"

class RetrImage
{
    Image image;
    GeoLabel labeledImg;
    vector<SuperPixel *> superPixelList;
    void buildSuperPixels();
public:
    RetrImage(string imgName);
    void show();
    vector<SuperPixel *> *getSuperPixels();
    string matchLabel(int id);
};

#endif // RETRIMAGE_H
