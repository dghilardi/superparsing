#ifndef SUPERVOXEL_H
#define SUPERVOXEL_H

#include <vector>
#include <map>
#include "../OpenCVGraphSegmentation/Segmentation.h"
#include "../superpixel.h"
#include "../Utils3d/tridimensionalvoxel.h"

using namespace std;
class SuperVoxel
{
    vector<SuperPixel *> superPixelsList;
    set<SuperVoxel *> adiacentsSet;
    int label;
    int firstFrame;
public:
    SuperVoxel(vector<Pixel> &pixelList, vector<cv::Mat> &frames);
    ~SuperVoxel();

    vector<SuperPixel *> *getSuperPixels();
    int getLabel();
    int getFirstFrameIdx();
    void setLabel(int newLabel);
    void show();

    void appendAdiacent(SuperVoxel *toAppend);
    const set<SuperVoxel *> *getAdiacents();

    static void computePerFrameNeighbour(vector<SuperVoxel *> superVoxelList, int height, int width, int nframes);
};

#endif // SUPERVOXEL_H
