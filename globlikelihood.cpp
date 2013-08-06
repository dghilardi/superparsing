#include "globlikelihood.h"

/**
 * @brief GlobLikelihood::getBestLabel
 * @return La migliore feature
 */
int GlobLikelihood::getBestLabel(){
    vector<int> keys;
    mask.getKeys(keys);
    double maxSum=0;
    int bestLabel;
    for(int i=0; i<keys.size(); ++i){
        double logSum = mask.getLogLikelihood(keys[i]) + relH.getLogLikelihood(keys[i]) + quantSIFT.getLogLikelihood(keys[i]) + colorHist.getLogLikelihood(keys[i]);
        if(logSum>maxSum){
            maxSum = logSum;
            bestLabel = keys[i];
        }
    }
    return bestLabel;
}
