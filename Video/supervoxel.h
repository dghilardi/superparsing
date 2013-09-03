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
public:
    SuperVoxel(vector<Pixel> &pixelList, vector<cv::Mat> &frames);
    ~SuperVoxel();

    const map<int, SuperPixel *> *getSuperPixels();
    void show();
};

#endif // SUPERVOXEL_H
