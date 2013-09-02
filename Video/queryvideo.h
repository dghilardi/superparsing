#ifndef QUERYVIDEO_H
#define QUERYVIDEO_H

#include <opencv/highgui.h>
#include "../OpenCVGraphSegmentation/Segmentation.h"
#include "video.h"

class QueryVideo
{
    Video video;
public:
    QueryVideo(string path);
};

#endif // QUERYVIDEO_H
