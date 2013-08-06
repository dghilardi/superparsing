#ifndef RETRIEVALSET_H
#define RETRIEVALSET_H

#include <string>
#include <vector>

#include "queryimage.h"
#include "superpixel.h"
#include "classlikelihood.h"

struct results {
    ClassLikelihood mask;
    ClassLikelihood relH;
    ClassLikelihood quantSIFT;
    ClassLikelihood colorHist;
    results() : mask("mask"), relH("relH"), quantSIFT("quantSIFT"), colorHist("colorHist") {}
};

using namespace std;
class RetrievalSet
{
    vector<string> imgNames;
public:
    RetrievalSet(vector<string> &names);
    void LabelImg(QueryImage &toLabel);
};

#endif // RETRIEVALSET_H
