#ifndef QUERYVIDEO_H
#define QUERYVIDEO_H

#include <vector>

#include <opencv/highgui.h>
#include "../OpenCVGraphSegmentation/Segmentation.h"
#include "video.h"
#include "supervoxel.h"
#include "Utils/printutils.h"
#include "Utils3d/tridimensionalvoxel.h"
#include "Utils3d/geometryengine.h"
#include "Utils3d/openglwindow.h"
#include <QApplication>
class QueryVideo
{
    Video video;
    vector<SuperVoxel *> superVoxelsList;
    vector<TridimensionalVoxel *> glVoxelsList;
    vector<GeometryEngine *> geoEngines;
    cv::Size vidSize;

    void showSegmentation(vector<vector<Pixel> > &coordList, int height, int width, int nframes);
public:
    QueryVideo(string path);
    ~QueryVideo();

    vector<SuperVoxel *> *getSuperVoxelsList();
    vector<GeometryEngine *> *showTridimensionalVoxels();
};

#endif // QUERYVIDEO_H
