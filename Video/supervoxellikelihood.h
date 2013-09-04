#ifndef SUPERVOXELLIKELIHOOD_H
#define SUPERVOXELLIKELIHOOD_H
#include <boost/thread.hpp>
#include <map>
#include <cmath>
#include "../globlikelihood.h"
#include "../debugHeader.h"

class SuperVoxelLikelihood
{
    struct WeightLikelihood{ //I save the relative weight of the superpixel for the computation of Edata
        double relativeWeight;
        GlobLikelihood *likelihood;
    };
    boost::mutex mtx;

    double sigmoid(double x);

    map<int, WeightLikelihood> globLikPerFrame;
public:
    SuperVoxelLikelihood();
    ~SuperVoxelLikelihood();
    GlobLikelihood *getGlobLikelihood(int frameIndex, double relWeight);
    double computeEdata(int labelIndex);
};

#endif // SUPERVOXELLIKELIHOOD_H
