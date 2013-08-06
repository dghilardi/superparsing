#ifndef CLASSLIKELIHOOD_H
#define CLASSLIKELIHOOD_H

#include <map>
#include <cmath>
#include <string>

using namespace std;
class ClassLikelihood
{
    map<int,int> numMatches; //Maps the number of the class to the number of matches founded for this superpixel
    static map<string, map<int,int> > totNum; //Maps the number of the class to the total number of superpixels of that class
    string featureName;
public:
    ClassLikelihood(string feature);
    static void incTotClass(int classID, string feature);
    void foundMatch(int classID);
    double getLogLikelihood(int classID);
};
#endif // CLASSLIKELIHOOD_H