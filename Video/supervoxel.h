#ifndef SUPERVOXEL_H
#define SUPERVOXEL_H

#include <vector>
#include <map>
#include "../OpenCVGraphSegmentation/Segmentation.h"
#include "../superpixel.h"

using namespace std;
class SuperVoxel
{
    map<int, SuperPixel *> superPixelsList;
    int label;
public:
    SuperVoxel(vector<Pixel> &pixelList, vector<cv::Mat> &frames);
    ~SuperVoxel();

    map<int, SuperPixel *> *getSuperPixels();
    int getLabel();
    void setLabel(int newLabel);
    void show();
};

#endif // SUPERVOXEL_H
