#ifndef MRF_H
#define MRF_H

#include "debugHeader.h"

#include <dai/var.h>
#include <dai/factor.h>
#include <dai/factorgraph.h>
#include <vector>
#include <cmath>

#include "superpixel.h"
#include "geolabel.h"
#include "globlikelihood.h"
#include "neighbourstat.h"

using namespace std;
class MRF
{
    static double sigmoid(double x);
public:
    MRF();
    static void computeMRF(vector<SuperPixel *> &superPixelList, GlobLikelihood &likelihood, NeighbourStat condprob);
};

#endif // MRF_H
