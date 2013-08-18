#include "globlikelihood.h"

/**
 * @brief GlobLikelihood::getBestLabel For each label compute the sum of the log of the probability that the superpixel is in that class and return the greatest value
 * @return La migliore feature
 */
int GlobLikelihood::getBestLabel(){
    vector<int> keys;
    mask.getKeys(keys);
    double maxSum=0;
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
    return mask.getLogLikelihood(i) + relH.getLogLikelihood(i) + quantSIFT.getLogLikelihood(i) + colorHist.getLogLikelihood(i);
}
