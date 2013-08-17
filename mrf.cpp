#include "mrf.h"

MRF::MRF()
{
}

void MRF::computeMRF(vector<SuperPixel *> &superPixelList){
    vector<dai::Var *> variables;
    vector<dai::Factor *> factors;

    int nlabels = GeoLabel::getLabelsNumber();
    //Inizialize a var for each superpixel
    for(int i=0; i<superPixelList.size(); ++i){
        variables.push_back(new dai::Var(i,nlabels));
    }

    for(int i=0; i<superPixelList.size(); ++i){
        SuperPixel *actual = superPixelList[i];
        const set<SuperPixel *> *adiacents = actual->getAdiacents();
        for(set<SuperPixel *>::iterator it=adiacents->begin(); it!=adiacents->end(); ++it){
            vector<SuperPixel *>::iterator itFound = std::find(superPixelList.begin(), superPixelList.end(), *it);
            int position = std::distance(superPixelList.begin(), itFound);
            assert(position<superPixelList.size());

        }
    }


}
