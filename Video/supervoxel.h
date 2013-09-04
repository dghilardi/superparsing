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
    set<SuperVoxel *> adiacentsSet;
    int label;
public:
    SuperVoxel(vector<Pixel> &pixelList, vector<cv::Mat> &frames);
    ~SuperVoxel();

    map<int, SuperPixel *> *getSuperPixels();
    int getLabel();
    void setLabel(int newLabel);
    void show();

    void appendAdiacent(SuperVoxel *toAppend);
    const set<SuperVoxel *> *getAdiacents();

    static void computePerFrameNeighbour(vector<SuperVoxel *> superVoxelList, int height, int width, int nframes);
};

#endif // SUPERVOXEL_H
