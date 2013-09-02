#ifndef SUPERVOXEL_H
#define SUPERVOXEL_H

#include <vector>
#include <map>
#include "../OpenCVGraphSegmentation/Segmentation.h"
#include "../superpixel.h"

using namespace std;
class SuperVoxel
{
    map<int, SuperPixel> superPixelsList;
public:
    SuperVoxel(vector<Pixel> &pixelList, vector<IplImage *> &frames);
    const map<int, SuperPixel> *getSuperPixels();
};

#endif // SUPERVOXEL_H
