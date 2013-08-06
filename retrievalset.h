#ifndef RETRIEVALSET_H
#define RETRIEVALSET_H

#include <string>
#include <vector>

#include "queryimage.h"
#include "superpixel.h"
#include "classlikelihood.h"
#include "globlikelihood.h"
#include "retrimage.h"

using namespace std;
class RetrievalSet
{
    vector<string> imgNames;
    vector<GlobLikelihood *> matchResults;
    void checkSuperPixel(SuperPixel *toLabel, SuperPixel *inSet, GlobLikelihood &spixelResults);
public:
    RetrievalSet(vector<string> &names);
    void LabelImg(QueryImage &imgToLabel);
};

#endif // RETRIEVALSET_H
