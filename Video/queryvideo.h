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
public:
    QueryVideo(string path);
    ~QueryVideo();
};

#endif // QUERYVIDEO_H
