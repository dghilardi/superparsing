#ifndef VISUALUTILS_H
#define VISUALUTILS_H

#include <opencv2/highgui/highgui.hpp>
#include <set>
#include <iostream>

#include "debugHeader.h"

#include "geolabel.h"

using namespace std;
class VisualUtils
{
    static void hslToRgb(cv::Vec3f hsl, cv::Vec3b &rgb);
public:
    VisualUtils();
    static void findDistinctColor(int index, int total, cv::Vec3b &result);
    static void colorLabels(cv::Mat &labeled, cv::Mat &result, set<int> &labels);
};

#endif // VISUALUTILS_H
