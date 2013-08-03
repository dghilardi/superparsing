#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <opencv2/highgui/highgui.hpp>

#include "OpenCVGraphSegmentation/Segmentation.h"

#define IMG_PATH "../SiftFlowDataset/Images/spatial_envelope_256x256_static_8outdoorcategories/"
#define IMG_EXT ".jpg"

using namespace std;

class Image
{
    cv::Mat image;
    string imgName;
protected:
public:
    Image(string filename);
    cv::Mat *getImage();
    void show();

};

#endif // IMAGE_H
