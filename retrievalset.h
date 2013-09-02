#ifndef RETRIEVALSET_H
#define RETRIEVALSET_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "mrf.h"
#include "queryimage.h"
#include "superpixel.h"
#include "classlikelihood.h"
#include "globlikelihood.h"
#include "retrimage.h"
#include "neighbourstat.h"
#include "Utils/threadsafestringset.h"

#include "globalSettings.h"
#include "debugHeader.h"

#define ERROR_PARSE_JSON 1
#define MSG_ERROR_PARSE_JSON "ERROR! The file doesn't exists or is malformed "

using namespace std;
class RetrievalSet
{
    boost::mutex neigTraining;
    Json::Value trainingSet;
    int trainingPos;
    vector<GlobLikelihood *> matchResults;
    static void checkSuperPixel(SuperPixel *toLabel, SuperPixel *inSet, GlobLikelihood &spixelResults);
    void applyMRF(QueryImage &imgToLabel, NeighbourStat &stat);
    string getNextTrainingPath();
    static void training(NeighbourStat *result, RetrievalSet *setInstance);
    static void computeLabels(QueryImage *imgToLabel, ThreadSafeStringSet *imgNames, vector<GlobLikelihood *> *matchResults);
public:
    RetrievalSet();
    ~RetrievalSet();
    void computeInstance(string instancePath, NeighbourStat &stat, bool useMRF, int nThreads);
    void computeNeighbourStatistics(NeighbourStat &result, string imgListPath, int threadNum);
    void assignLabels(QueryImage &query);
};

#endif // RETRIEVALSET_H
