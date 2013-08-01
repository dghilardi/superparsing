#ifndef GEOLABEL_H
#define GEOLABEL_H

#include <string>
#include <vector>
#include <matio.h>
#include <iostream>

#include "opencv2/highgui/highgui.hpp"

#define ERROR_LOADING_MAT 1
#define ERROR_LABEL_INDEX 2

#define MSG_ERROR_LABEL_INDEX "ERROR! there is no label for index: "

#define NO_LABEL "none"
using namespace std;

class GeoLabel
{
    cv::Mat imageLabeled;
    vector<string *> labels;
    string imgName;
public:
    GeoLabel(string filename);
    cv::Mat *getLabeledImg();
    string getLabel(int index);
    void viewLabeledImg();
};

#endif // READDATA_H
