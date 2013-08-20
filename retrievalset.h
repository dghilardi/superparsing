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
    void checkSuperPixel(SuperPixel *toLabel, SuperPixel *inSet, GlobLikelihood &spixelResults);
    void applyMRF(QueryImage &imgToLabel, NeighbourStat &stat);
    string getNextTrainingPath();
    static void training(NeighbourStat *result, RetrievalSet *setInstance);
public:
    RetrievalSet();
    ~RetrievalSet();
    void computeInstance(string instancePath, NeighbourStat &stat, bool useMRF);
    void LabelImg(QueryImage &imgToLabel, vector<string> &imgNames);
    void computeNeighbourStatistics(NeighbourStat &result, string imgListPath, int threadNum);
};

#endif // RETRIEVALSET_H
