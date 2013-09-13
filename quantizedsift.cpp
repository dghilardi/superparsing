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

double QuantizedSift::distance(int descrA, int descB){}

void QuantizedSift::computeKmean(cv::Mat &wholeDescriptors){
    //Construct BOWKMeansTrainer
    //the number of bags
    int dictionarySize=100;
    //define Term Criteria
    cv::TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);
    //retries number
    int retries=1;
    //necessary flags
    int flags=cv::KMEANS_PP_CENTERS;
    //Create the BoW (or BoF) trainer
    cv::BOWKMeansTrainer bowTrainer(dictionarySize,tc,retries,flags);
    //cluster the feature vectors
    cv::Mat dictionary=bowTrainer.cluster(wholeDescriptors);
    //store the vocabulary
    cv::FileStorage fs("dictionary.yml", cv::FileStorage::WRITE);
    fs << "vocabulary" << dictionary;
    fs.release();
}
