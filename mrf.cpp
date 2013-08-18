#include "mrf.h"
#define THETA 1
MRF::MRF()
{
}

void MRF::computeMRF(vector<SuperPixel *> &superPixelList, GlobLikelihood &likelihood, NeighbourStat condprob){
    vector<dai::Var> variables;
    vector<dai::Factor> factors;

    int nlabels = GeoLabel::getLabelsNumber();
    //Inizialize a var for each superpixel
    for(int i=0; i<superPixelList.size(); ++i){
        dai::Var var(i,nlabels);
        variables.push_back(var);
    }

    for(int i=0; i<superPixelList.size(); ++i){
        SuperPixel *actual = superPixelList[i];
        dai::Factor eData(variables[i]);

        //set possible values for Edata
        for(int state=0; state<nlabels; ++state){
            eData.set(state, -sigmoid(likelihood.getLogSum(state+1)));
        }
        factors.push_back(eData);

        //get all adiacents
        const set<SuperPixel *> *adiacents = actual->getAdiacents();
        for(set<SuperPixel *>::iterator it=adiacents->begin(); it!=adiacents->end(); ++it){
            vector<SuperPixel *>::iterator itFound = std::find(superPixelList.begin(), superPixelList.end(), *it);
            int position = std::distance(superPixelList.begin(), itFound);
            assert(position<superPixelList.size());

            //Esmooth
            dai::Factor eSmooth(dai::VarSet(variables[i], variables[position]));
            for(int stateVar=0; stateVar<nlabels; ++stateVar){ for(int stateAdiacent=0; stateAdiacent<nlabels; ++stateAdiacent){
                    double prob = condprob.conditionalNeigProb(stateVar+1, stateAdiacent+1)+condprob.conditionalNeigProb(stateAdiacent+1, stateVar+1)/2;
                    eSmooth.set(stateVar*nlabels+stateAdiacent, -log(prob));
            }}
            factors.push_back(eSmooth);
        }
    }

    dai::FactorGraph graph(factors);
}

double MRF::sigmoid(double x){
    return exp(THETA*x)/(1+exp(THETA*x));
}

