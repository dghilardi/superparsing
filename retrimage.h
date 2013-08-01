#ifndef RETRIMAGE_H
#define RETRIMAGE_H

#include "image.h"
#include "geolabel.h"

class RetrImage
{
    Image image;
    GeoLabel labeledImg;
public:
    RetrImage(string imgName): image(imgName), labeledImg(imgName) {}
    void show();
};

#endif // RETRIMAGE_H
