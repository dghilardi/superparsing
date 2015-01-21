#include "supervoxellikelihood.h"
#include "../debugHeader.h"
#define GAMMA 1.0
SuperVoxelLikelihood::SuperVoxelLikelihood()
{
}

SuperVoxelLikelihood::~SuperVoxelLikelihood(){
    for(map<int, GlobLikelihood *>::iterator it=globLikPerFrame.begin(); it!=globLikPerFrame.end(); ++it){
        delete it->second;
    }
}

GlobLikelihood *SuperVoxelLikelihood::getGlobLikelihood(int frameIndex, double relWeight){
    mtx.lock();
    if(!globLikPerFrame.count(frameIndex)){
        globLikPerFrame[frameIndex] = new GlobLikelihood;
        globLikPerFrame[frameIndex]->setWeight(relWeight);
    }
    mtx.unlock();
    return globLikPerFrame[frameIndex];
}

/**
 * @brief SuperVoxelLikelihood::computeEdata Compute EData as the maximum EData for each superpixel contained in the supervoxel
 * @param labelIndex
 * @return
 */
double SuperVoxelLikelihood::computeEdata(int labelIndex){
    double max=0;
    for(map<int, GlobLikelihood *>::iterator it=globLikPerFrame.begin(); it!=globLikPerFrame.end(); ++it){
        double tmpVal= it->second->computeEdata(labelIndex);
        if(tmpVal>max){
            max=tmpVal;
        }
    }
    return max;
}

int SuperVoxelLikelihood::getBestLabel(){
    int nlabels = GeoLabel::getLabelsNumber();
    assert(nlabels>0);
    double max=0;
    int maxIndex;
    for(int i=0; i<nlabels; ++i){
        double actualEdata = computeEdata(i);
        if(actualEdata>max){
            max=actualEdata;
            maxIndex = i;
        }
    }
    return maxIndex;
}
