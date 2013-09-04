#include "supervoxellikelihood.h"

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
