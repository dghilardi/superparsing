#include "quantizedsift.h"

QuantizedSift::QuantizedSift()
{
}

QuantizedSift::~QuantizedSift(){
    //delete dictionary
}

int QuantizedSift::getIndex(cv::Mat &descr){
    int minIndex = 0;
    vector<cv::Mat> matches;
    //cv::BruteForceMatcher<cv::L2> matcher;
    //matcher.match(descr, *(dictionary[0]), matches);
    for(int i=1; i<dictionary.size(); ++i){

    }
    return minIndex;
}

double distance(int descrA, int descB);
void computeKmean(string jsonPath);
