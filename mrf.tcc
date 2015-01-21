#include "mrf.h"
#include "debugHeader.h"
template<typename MRFNode, typename Likelihood>
void MRF::computeMRF(vector<MRFNode *> superPixelList, vector<Likelihood *> &likelihood, NeighbourStat &condprob){
    vector<dai::Var> variables;
    vector<dai::Factor> factors;

    int nlabels = GeoLabel::getLabelsNumber();
    //Inizialize a var for each superpixel
    for(uint i=0; i<superPixelList.size(); ++i){
        dai::Var var(i,nlabels);
        variables.push_back(var);
    }

    //cout << "Labeling states:" << endl;
    for(uint i=0; i<superPixelList.size(); ++i){
        PrintUtils::printPercentage(i, superPixelList.size());
        MRFNode *actual = superPixelList[i];
        dai::Factor eData(variables[i]);

        //set possible values for Edata
        for(int state=0; state<nlabels; ++state){
            eData.set(state, exp(likelihood[i]->computeEdata(state+1)));
        }
        factors.push_back(eData);

        //get all adiacents
        const set<MRFNode *> *adiacents = actual->getAdiacents();
        for(typename set<MRFNode *>::iterator it=adiacents->begin(); it!=adiacents->end(); ++it){
            typename vector<MRFNode *>::iterator itFound = std::find(superPixelList.begin(), superPixelList.end(), *it);
            uint position = std::distance(superPixelList.begin(), itFound);
            assert(position<superPixelList.size());

            //Esmooth
            dai::Factor eSmooth(dai::VarSet(variables[i], variables[position]));
            for(int stateVar=0; stateVar<nlabels; ++stateVar){ for(int stateAdiacent=0; stateAdiacent<nlabels; ++stateAdiacent){
                    if(stateVar!=stateAdiacent){
                        double prob = (condprob.conditionalNeigProb(stateVar+1, stateAdiacent+1)+condprob.conditionalNeigProb(stateAdiacent+1, stateVar+1))/2;
                        //prob=1-prob;
                        prob= (prob==0) ? 1e-6 : prob;
                        assert(prob>0 && prob<=1);
                        eSmooth.set(stateVar*nlabels+stateAdiacent, exp(-log(prob)));
                    }else{
                        eSmooth.set(stateVar*nlabels+stateAdiacent, 1);
                    }
            }}
            factors.push_back(eSmooth);
        }
    }
    //cout << "OK" << endl;

    dai::FactorGraph graph(factors);
    vector<size_t> result;
    computeMAP(graph, result);

    //Stampo risultati
    double funcValue = 0;
    double funcValuePrev = 0;
    for(int i=0;i<result.size(); ++i){
        MRFNode *actual = superPixelList[i];
        //cout << i << ":\t" << actual->getLabel() <<" -> "<< result[i]+1 << endl;
        superPixelList[i]->setLabel(result[i]+1);
        //funcValue += -actual->getWeight()*sigmoid(likelihood[i]->getLogSum(result[i]+1));
        //funcValuePrev += -actual->getWeight()*sigmoid(likelihood[i]->getLogSum(actual->getLabel()));

        funcValue += likelihood[i]->computeEdata(result[i]+1);
        funcValuePrev += likelihood[i]->computeEdata(actual->getLabel());
        const set<MRFNode *> *adiacents = actual->getAdiacents();
        for(typename set<MRFNode *>::iterator it=adiacents->begin(); it!=adiacents->end(); ++it){
            typename vector<MRFNode *>::iterator itFound = std::find(superPixelList.begin(), superPixelList.end(), *it);
            uint position = std::distance(superPixelList.begin(), itFound);
            if(result[i]!=result[position]){
                double prob = condprob.conditionalNeigProb(result[i]+1, result[position]+1)+condprob.conditionalNeigProb(result[position]+1, result[i]+1)/2;
                prob= (prob==0) ? 1e-9 : prob;
                funcValue += -log(prob);
            }
            if(actual->getLabel()!=(*it)->getLabel()){
                double prob = condprob.conditionalNeigProb(actual->getLabel(), (*it)->getLabel())+condprob.conditionalNeigProb((*it)->getLabel(), actual->getLabel())/2;
                prob= (prob==0) ? 1e-9 : prob;
                funcValue += -log(prob);
            }
        }
    }
    //cout << "Valore J(c) [prima]="<<funcValuePrev<<endl;
    //cout << "Valore J(c)  [dopo]="<<funcValue<<endl;
}

#define SCALE_FACTOR 10000
template<typename MRFNode, typename Likelihood>
void MRF::computeMRFGCO(vector<MRFNode *> superPixelList, vector<Likelihood *> &likelihood, NeighbourStat &condprob){
    int numSuperPixels = superPixelList.size();
    int numLabels = GeoLabel::getLabelsNumber();

    int *result = new int[numSuperPixels];

    //Create data costs
    int *data = new int[numSuperPixels*numLabels]();
    for(uint i=0; i<numSuperPixels; ++i){
        for(int state=0; state<numLabels; ++state){
            data[i*numLabels+state] = -likelihood[i]->computeEdata(state+1)*SCALE_FACTOR;
            //cout << "Data " << i*numLabels+state << ":\t" << data[i*numLabels+state] << endl;
        }
    }

    //Create array for smooth costs
    int *smooth = new int[numLabels*numLabels]();
    for(int l2=0; l2<numLabels; ++l2){
        for(int l1=0; l1<numLabels; ++l1){            
            double prob = (condprob.conditionalNeigProb(l1+1, l2+1)+condprob.conditionalNeigProb(l2+1, l1+1))/2.0;
            prob = (1.0-prob);
            if(l1==l2)
                prob=0;
            prob= (prob==0) ? 1e-3 : prob;
            smooth[l1+l2*numLabels] = (int)(log(prob)*SCALE_FACTOR/500); //coeff piccolo -> soglia alta/compressione
            //cout << "Smooth " << l1+l2*numLabels << ":\t" << smooth[l1+l2*numLabels] << endl;
            //if(l1==l2)
                //smooth[l1+l2*numLabels] = -1400;
            /*
            if(l1==l2)
                smooth[l1+l2*numLabels] = -100;
            else
                smooth[l1+l2*numLabels] = 0;
            */
        }
    }/*
    for(int i=0; i<numSuperPixels*numLabels; ++i){
        cout<<"data: "<<data[i]<<endl;
    }
    for(int i=0; i<numLabels*numLabels; ++i){
        cout<<"smooth: "<<smooth[i]<<endl;
    }*/
    try{
        GCoptimizationGeneralGraph *gc = new GCoptimizationGeneralGraph(numSuperPixels, numLabels);
        gc->setDataCost(data);
        gc->setSmoothCost(smooth);
        set<std::pair<int,int> > adiacentSet;

        //Set up neighborhood system
        for(uint i=0; i<numSuperPixels; ++i){
            MRFNode *actual = superPixelList[i];

            const set<MRFNode *> *adiacents = actual->getAdiacents();
            for(typename set<MRFNode *>::iterator it=adiacents->begin(); it!=adiacents->end(); ++it){
                typename vector<MRFNode *>::iterator itFound = std::find(superPixelList.begin(), superPixelList.end(), *it);
                uint position = std::distance(superPixelList.begin(), itFound);
                assert(position<numSuperPixels);
                pair<int, int> adjPair(max(i,position),min(i,position));
                adiacentSet.insert(adjPair);
            }

        }
        for(set<std::pair<int,int> >::iterator it=adiacentSet.begin(); it!=adiacentSet.end(); ++it ){
            gc->setNeighbors(it->first, it->second);
        }        
        //gc->expansion(2);
        printf("\nBefore optimization energy is %d",gc->compute_energy());
        gc->swap();
        printf("\nAfter optimization energy is %d\n",gc->compute_energy());

        for(int i=0; i<numSuperPixels; ++i)
            result[i] = gc->whatLabel(i);
        delete gc;
    }catch(GCException e){
        e.Report();
    }

    for(int i=0; i<numSuperPixels; ++i){
        superPixelList[i]->setLabel(result[i]+1);
    }


    delete [] result;
    delete [] smooth;
    delete [] data;
}
