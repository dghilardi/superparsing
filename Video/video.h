#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include "../debugHeader.h"

using namespace std;
class Video
{
    cv::VideoCapture vid;
public:
    Video(string path);
    void getImages(vector<cv::Mat> &result, int maxNumber);
};

#endif // VIDEO_H
