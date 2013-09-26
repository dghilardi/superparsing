#include "globlikelihood.h"

#define THETA 1

/**
 * @brief  GlobLikelihood::getBestLabel For each label compute the sum of the log of the probability that the superpixel is in that class and return the greatest value
 * @return La migliore feature
 */
int GlobLikelihood::getBestLabel(){
    vector<int> keys;
    mask.getKeys(keys);
    double maxSum=std::numeric_limits<double>::min();
    int bestLabel=0;
    for(int i=0; i<keys.size(); ++i){
        double logSum = getLogSum(keys[i]);
        if(logSum>maxSum){
            maxSum = logSum;
            bestLabel = keys[i];
        }
    }   
    return bestLabel;
}

double GlobLikelihood::getLogSum(int i){
    float w[] = {2, 2, 1, 1};
    return  w[0]*mask.getLogLikelihood(i) + w[1]*relH.getLogLikelihood(i) + w[2]*quantSIFT.getLogLikelihood(i) + w[3]*colorHist.getLogLikelihood(i);
}

void GlobLikelihood::setWeight(double newWeight){
    weight = newWeight;
}

double GlobLikelihood::computeEdata(int index){
    return weight*sigmoid(getLogSum(index));
}

double GlobLikelihood::sigmoid(double x){
    return exp(THETA*x)/(1+exp(THETA*x));
}
