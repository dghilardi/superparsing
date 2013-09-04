#include "mrf.h"
#define THETA 1
MRF::MRF()
{
}

// FOR computeMRF see mrf.tcc

void MRF::computeMAP(dai::FactorGraph &graph, vector<size_t> &decmapstate){
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

    //vector<size_t> mpstate = mp.findMaximum();

    dai::DecMAP decmap(graph, opts("reinit",true)("ianame",string("BP"))("iaopts",string("[damping=0.1,inference=MAXPROD,logdomain=0,maxiter=1000,tol=1e-9,updates=SEQRND,verbose=1]")) );
    decmap.init();
    decmap.run();
    decmapstate = decmap.findMaximum();
}

double MRF::sigmoid(double x){
    return exp(THETA*x)/(1+exp(THETA*x));
}

