#include "supervoxellikelihood.h"

SuperVoxelLikelihood::SuperVoxelLikelihood()
{
}

SuperVoxelLikelihood::~SuperVoxelLikelihood(){
    for(map<int, GlobLikelihood *>::iterator it=globLikPerFrame.begin(); it!=globLikPerFrame.end(); ++it){
        delete it->second;
    }
}

GlobLikelihood *SuperVoxelLikelihood::getGlobLikelihood(int frameIndex){
    if(!globLikPerFrame.count(frameIndex)){
        globLikPerFrame[frameIndex] = new GlobLikelihood;
    }
    return globLikPerFrame[frameIndex];
}
