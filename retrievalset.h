#ifndef RETRIEVALSET_H
#define RETRIEVALSET_H

#include <string>
#include <vector>

#include "queryimage.h"
#include "superpixel.h"
#include "classlikelihood.h"
#include "globlikelihood.h"
#include "retrimage.h"
#include "neighbourstat.h"

#define ERROR_PARSE_JSON 1
#define MSG_ERROR_PARSE_JSON "ERROR! The file doesn't exists or is malformed "

using namespace std;
class RetrievalSet
{
    vector<string> imgNames;
    vector<GlobLikelihood *> matchResults;
    NeighbourStat statNeig;
    void checkSuperPixel(SuperPixel *toLabel, SuperPixel *inSet, GlobLikelihood &spixelResults);
public:
    RetrievalSet();
    void computeInstance(string instancePath);
    void LabelImg(QueryImage &imgToLabel);
};

#endif // RETRIEVALSET_H
