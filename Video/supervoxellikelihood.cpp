#include "supervoxellikelihood.h"

#define GAMMA 1.0
SuperVoxelLikelihood::SuperVoxelLikelihood()
{
}

SuperVoxelLikelihood::~SuperVoxelLikelihood(){
    for(map<int, WeightLikelihood>::iterator it=globLikPerFrame.begin(); it!=globLikPerFrame.end(); ++it){
        delete it->second.likelihood;
    }
}

GlobLikelihood *SuperVoxelLikelihood::getGlobLikelihood(int frameIndex, double relWeight){
    mtx.lock();
    if(!globLikPerFrame.count(frameIndex)){
        globLikPerFrame[frameIndex].likelihood = new GlobLikelihood;
        globLikPerFrame[frameIndex].relativeWeight = relWeight;
    }else{
        assert(globLikPerFrame[frameIndex].relativeWeight == relWeight);
    }
    mtx.unlock();
    return globLikPerFrame[frameIndex].likelihood;
}

double SuperVoxelLikelihood::computeEdata(int labelIndex){
    double max=0;
    for(map<int, WeightLikelihood>::iterator it=globLikPerFrame.begin(); it!=globLikPerFrame.end(); ++it){
        double tmpVal= it->second.relativeWeight*sigmoid(it->second.likelihood->getLogSum(labelIndex));
        if(tmpVal>max){
            max=tmpVal;
        }
    }
    return -max;
}

double SuperVoxelLikelihood::sigmoid(double x){
    return exp(GAMMA*x)/(1+exp(GAMMA*x));
}
