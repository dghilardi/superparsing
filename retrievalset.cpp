#include "retrievalset.h"

RetrievalSet::RetrievalSet(vector<string> &names) : imgNames(names), matchResults()
{
}

void RetrievalSet::LabelImg(QueryImage &imgToLabel){
    vector<SuperPixel *> *setSuperPixelsToLabel = imgToLabel.getSuperPixels();
    for(uint i=0; i<setSuperPixelsToLabel->size(); ++i) matchResults.push_back(new GlobLikelihood());
    for(uint i=0; i<imgNames.size(); ++i){
        RetrImage setImage(imgNames[i]);
        vector<SuperPixel *> *setImgSuperPixels = setImage.getSuperPixels();
        for(uint j=0; j<setImgSuperPixels->size(); ++j){
            SuperPixel *setSuperPixel = (*setImgSuperPixels)[j];
            for(uint k=0; k<setSuperPixelsToLabel->size(); ++k){
                SuperPixel *superPixelToLabel = (*setSuperPixelsToLabel)[k];
                checkSuperPixel(superPixelToLabel, setSuperPixel, *(matchResults[k]));
            }
        }
    }

    for(uint k=0; k<setSuperPixelsToLabel->size(); ++k){
        SuperPixel *superPixelToLabel = (*setSuperPixelsToLabel)[k];
        superPixelToLabel->setLabel(matchResults[k]->getBestLabel());
    }

    for(int i=0; i<matchResults.size(); ++i) delete matchResults[i];
}

void RetrievalSet::checkSuperPixel(SuperPixel *toLabel, SuperPixel *inSet, GlobLikelihood &spixelResults){
    double tk_mask=10, tk_relH=0.2, tk_sift=1, tk_color=1;

    int actualLabel = inSet->getLabel();

    double maskDistance = toLabel->getMaskDistance(*inSet);
    spixelResults.mask.incTotClass(actualLabel);
    if(maskDistance<tk_mask) spixelResults.mask.foundMatch(actualLabel);

    double relHDistance = toLabel->getRelHeightDistance(*inSet);
    spixelResults.relH.incTotClass(actualLabel);
    if(relHDistance<tk_relH) spixelResults.relH.foundMatch(actualLabel);

    double siftDistance = toLabel->getSiftDistance(*inSet);
    spixelResults.quantSIFT.incTotClass(actualLabel);
    if(siftDistance<tk_sift) spixelResults.quantSIFT.foundMatch(actualLabel);

    double colDistance = toLabel->getColorDistance(*inSet);
    spixelResults.colorHist.incTotClass(actualLabel);
    if(colDistance<tk_color) spixelResults.colorHist.foundMatch(actualLabel);
}
