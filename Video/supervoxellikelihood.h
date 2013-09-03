#ifndef SUPERVOXELLIKELIHOOD_H
#define SUPERVOXELLIKELIHOOD_H

#include <map>
#include "../globlikelihood.h"

class SuperVoxelLikelihood
{
    map<int, GlobLikelihood *> globLikPerFrame;
public:
    SuperVoxelLikelihood();
    ~SuperVoxelLikelihood();
    GlobLikelihood *getGlobLikelihood(int frameIndex);
};

#endif // SUPERVOXELLIKELIHOOD_H
