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

void MRF::computeMAP(dai::FactorGraph &graph){
    size_t maxStates = 1000000;
    size_t maxiter=1000;
    dai::Real tol = 1e-9;
    size_t verb = 1;

    dai::PropertySet opts;
    opts.set("maxiter", maxiter);
    opts.set("tol", tol);
    opts.set("verbose", verb);

    bool do_jt = true;
    try {
        dai::boundTreewidth(graph, &dai::eliminationCost_WeightedMinFill, maxStates);
    } catch(dai::Exception &e){
        if(e.getCode() == dai::Exception::OUT_OF_MEMORY){
            do_jt = false;
            cout << "Skipping junction tree (need more than " << maxStates << " states)." << endl;
        }
    }

    dai::JTree jt, jtmap;
    vector<size_t> jtmapstate;
    if(do_jt){
        jt=dai::JTree(graph, opts("updates", string("HUGIN")));
        jt.init();
        jt.run();

        jtmap = dai::JTree(graph, opts("updates", string("HUGIN"))("interference", string("MAXPROD")));
        jtmap.init();
        jtmap.run();
        jtmapstate = jtmap.findMaximum();
    }

    dai::BP bp(graph, opts("updates", string("SEQRND"))("logdomain", false));
    bp.init();
    bp.run();

    dai::BP mp(graph, opts("updates",string("SEQRND"))("logdomain",false)("inference",string("MAXPROD"))("damping",string("0.1")));
    mp.init();
    mp.run();

    vector<size_t> mpstate = mp.findMaximum();

    dai::DecMAP decmap(graph, opts("reinit",true)("ianame",string("BP"))("iaopts",string("[damping=0.1,inference=MAXPROD,logdomain=0,maxiter=1000,tol=1e-9,updates=SEQRND,verbose=1]")) );
    decmap.init();
    decmap.run();
    vector<size_t> decmapstate = decmap.findMaximum();

}

double MRF::sigmoid(double x){
    return exp(THETA*x)/(1+exp(THETA*x));
}

