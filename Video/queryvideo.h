#ifndef QUERYVIDEO_H
#define QUERYVIDEO_H

#include <vector>

#include <opencv/highgui.h>
#include "../OpenCVGraphSegmentation/Segmentation.h"
#include "video.h"
#include "supervoxel.h"

class QueryVideo
{
    Video video;
    vector<SuperVoxel *> superVoxelsList;

    void showSegmentation(vector<vector<Pixel> > &coordList, int height, int width, int nframes);
public:
    QueryVideo(string path);
    ~QueryVideo();

    vector<SuperVoxel *> *getSuperVoxelsList();
};

#endif // QUERYVIDEO_H
