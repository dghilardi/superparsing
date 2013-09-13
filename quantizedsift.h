#ifndef QUANTIZEDSIFT_H
#define QUANTIZEDSIFT_H
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/legacy/legacy.hpp>
using namespace std;
class QuantizedSift
{
    vector<cv::Mat *> dictionary;
public:
    QuantizedSift();
    virtual ~QuantizedSift();
    int getIndex(cv::Mat &descr);
    double distance(int descrA, int descB);
    void computeKmean(cv::Mat &wholeDescriptors);
};

#endif // QUANTIZEDSIFT_H
