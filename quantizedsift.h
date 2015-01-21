#ifndef QUANTIZEDSIFT_H
#define QUANTIZEDSIFT_H
#include <vector>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <cmath>
#include <cfloat>
#include <string>


#define QUANTIZATION_SIZE 100
using namespace std;
class QuantizedSift
{
    static QuantizedSift* instancePointer;
    QuantizedSift();
    cv::Mat dictionary;
public:
    virtual ~QuantizedSift();
    static QuantizedSift *getInstance(){
        if(instancePointer==NULL){
            instancePointer = new QuantizedSift();
        }
        return instancePointer;
    }

    int getIndex(cv::Mat descr);
    void computeKmean(cv::Mat &wholeDescriptors);
    void loadDictionary(cv::FileStorage fs);
};

#endif // QUANTIZEDSIFT_H
