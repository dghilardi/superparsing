#ifndef GLOBLIKELIHOOD_H
#define GLOBLIKELIHOOD_H

#include <vector>

#include "classlikelihood.h"

using namespace std;
class GlobLikelihood
{
public:
    GlobLikelihood(): mask("mask"), relH("relH"), quantSIFT("quantSIFT"), colorHist("colorHist") {}
    ClassLikelihood mask;
    ClassLikelihood relH;
    ClassLikelihood quantSIFT;
    ClassLikelihood colorHist;
    int getBestLabel();
    double getLogSum(int i);
};

#endif // GLOBLIKELIHOOD_H
