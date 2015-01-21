#include "quantizedsift.h"
#include "debugHeader.h"
QuantizedSift *QuantizedSift::instancePointer = NULL;
QuantizedSift::QuantizedSift()
{
}

QuantizedSift::~QuantizedSift(){
    //delete dictionary
}

int QuantizedSift::getIndex(cv::Mat descr){
    assert(descr.cols == 128);
    assert(descr.rows == 1);
    assert(descr.type() == CV_32F);

    assert(dictionary.cols == 128);
    assert(dictionary.rows == QUANTIZATION_SIZE);
    assert(dictionary.type() == CV_32F);


    int minDistanceIndex = -1;
    float minDistance = FLT_MAX;
    for(int i=0; i<dictionary.rows; ++i){
        float euclideanDistance = 0;
        for(int j=0; j<dictionary.cols; ++j){
            euclideanDistance += pow(descr.at<float>(0,j)-dictionary.at<float>(i,j),2);
        }
        if(euclideanDistance<minDistance){
            minDistance = euclideanDistance;
            minDistanceIndex = i;
        }
    }
    assert(minDistanceIndex!=-1);
    return minDistanceIndex;
}

void QuantizedSift::computeKmean(cv::Mat &wholeDescriptors){
    //Construct BOWKMeansTrainer
    //the number of bags
    int dictionarySize=QUANTIZATION_SIZE;
    //define Term Criteria
    cv::TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);
    //retries number
    int retries=1;
    //necessary flags
    int flags=cv::KMEANS_PP_CENTERS;
    //Create the BoW (or BoF) trainer
    cv::BOWKMeansTrainer bowTrainer(dictionarySize,tc,retries,flags);
    //cluster the feature vectors
    dictionary=bowTrainer.cluster(wholeDescriptors);
    //store the vocabulary
    cv::FileStorage fs("dictionary.yml", cv::FileStorage::WRITE);
    fs << "vocabulary" << dictionary;
    fs.release();
}

void QuantizedSift::loadDictionary(cv::FileStorage fs){
    fs["vocabulary"] >> dictionary;
}
