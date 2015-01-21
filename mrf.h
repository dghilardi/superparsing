#ifndef MRF_H
#define MRF_H

#include <dai/alldai.h>
#include <dai/jtree.h>
#include <dai/bp.h>
#include <dai/decmap.h>

#include <vector>
#include <map>
#include <cmath>

#include <iostream>

#include "superpixel.h"
#include "geolabel.h"
#include "neighbourstat.h"
#include "Utils/printutils.h"
#include "GCO/GCoptimization.h"



using namespace std;
class MRF
{
    static double sigmoid(double x);
    static void computeMAP(dai::FactorGraph &graph, vector<size_t> &decmapstate);
public:
    MRF();
    template<typename MRFNode, typename Likelihood>
        static void computeMRF(vector<MRFNode *> superPixelList, vector<Likelihood *> &likelihood, NeighbourStat &condprob);
    template<typename MRFNode, typename Likelihood>
        static void computeMRFGCO(vector<MRFNode *> superPixelList, vector<Likelihood *> &likelihood, NeighbourStat &condprob);
};
#include "mrf.tcc"

#endif // MRF_H
