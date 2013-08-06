#include "classlikelihood.h"

#define EPSILON 0.1

ClassLikelihood::ClassLikelihood(string feature){
    featureName = feature;
}

map<string, map<int,int> > ClassLikelihood::totNum;

void ClassLikelihood::incTotClass(int classID){
    ++totNum[featureName][classID];
}

void ClassLikelihood::foundMatch(int classID){
    ++numMatches[classID];
}

double ClassLikelihood::getLogLikelihood(int classID){
    int totNotC = 0;
    int matchNotC = 0;
    for(map<int,int>::iterator it = totNum[featureName].begin(); it!=totNum[featureName].end(); ++it) totNotC+=it->second;
    for(map<int,int>::iterator it = numMatches.begin(); it != numMatches.end(); ++it) matchNotC+=it->second;
    totNotC -= totNum[featureName][classID];
    matchNotC -= numMatches[classID];
    double frac1 = (totNum[featureName][classID]+EPSILON)/(double)(totNotC+EPSILON);
    double frac2 = matchNotC/(double)numMatches[classID];
    return log(frac1*frac2);
}

int ClassLikelihood::getLabelsNumber(){
    return totNum[featureName].size();
}

void ClassLikelihood::getKeys(vector<int> &keys){
    for(map<int,int>::iterator it = totNum[featureName].begin(); it!=totNum[featureName].end(); ++it) keys.push_back(it->first);
}
