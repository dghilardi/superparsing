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

    cout << "Labeling states:" << endl;
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
    cout << "OK" << endl;

    dai::FactorGraph graph(factors);
    vector<size_t> result;
    computeMAP(graph, result);

    //Stampo risultati
    double funcValue = 0;
    double funcValuePrev = 0;
    for(int i=0;i<result.size(); ++i){
        MRFNode *actual = superPixelList[i];
        cout << i << ":\t" << actual->getLabel() <<" -> "<< result[i]+1 << endl;
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
    cout << "Valore J(c) [prima]="<<funcValuePrev<<endl;
    cout << "Valore J(c)  [dopo]="<<funcValue<<endl;
}
