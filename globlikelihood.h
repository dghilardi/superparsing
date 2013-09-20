#ifndef GLOBLIKELIHOOD_H
#define GLOBLIKELIHOOD_H

#include <vector>
#include <limits>

#include "classlikelihood.h"

using namespace std;
class GlobLikelihood
{
    double weight;
    double sigmoid(double x);
public:
    GlobLikelihood(): mask("mask"), relH("relH"), quantSIFT("quantSIFT"), colorHist("colorHist"), weight(-1) {}
    void setWeight(double newWeight);
    ClassLikelihood mask;
    ClassLikelihood relH;
    ClassLikelihood quantSIFT;
    ClassLikelihood colorHist;
    int getBestLabel();
    double getLogSum(int i);
    double computeEdata(int index);
};

#endif // GLOBLIKELIHOOD_H
