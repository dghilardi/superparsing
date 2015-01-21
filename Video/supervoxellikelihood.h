#ifndef SUPERVOXELLIKELIHOOD_H
#define SUPERVOXELLIKELIHOOD_H
#include <boost/thread.hpp>
#include <map>
#include <cmath>
#include "../globlikelihood.h"
#include "../geolabel.h"


class SuperVoxelLikelihood
{
    boost::mutex mtx;
    map<int, GlobLikelihood *> globLikPerFrame;
public:
    SuperVoxelLikelihood();
    ~SuperVoxelLikelihood();
    GlobLikelihood *getGlobLikelihood(int frameIndex, double relWeight);
    double computeEdata(int labelIndex);
    int getBestLabel();
};

#endif // SUPERVOXELLIKELIHOOD_H
