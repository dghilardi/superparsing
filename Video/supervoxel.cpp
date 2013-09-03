#include "supervoxel.h"

SuperVoxel::SuperVoxel(vector<Pixel> &pixelList, vector<cv::Mat> &frames) : label(-1)
{
    map<int, vector<Pixel> > dividedPerFrame;
    for(uint i=0; i<pixelList.size(); ++i){
        dividedPerFrame[pixelList[i].f].push_back(pixelList[i]);
    }

    for(map<int, vector<Pixel> >::iterator it=dividedPerFrame.begin(); it!=dividedPerFrame.end(); ++it){
        superPixelsList.insert(map<int, SuperPixel *>::value_type(it->first, new SuperPixel(it->second, frames[it->first])));
    }
}

SuperVoxel::~SuperVoxel(){
    for(map<int, SuperPixel *>::iterator it=superPixelsList.begin(); it!=superPixelsList.end(); ++it){
        delete it->second;
    }
}

map<int, SuperPixel *> *SuperVoxel::getSuperPixels(){
    return &superPixelsList;
}

int SuperVoxel::getLabel(){
    return label;
}

void SuperVoxel::setLabel(int newLabel){
    label=newLabel;
}

void SuperVoxel::show(){
    for(map<int, SuperPixel *>::iterator it=superPixelsList.begin(); it!=superPixelsList.end(); ++it){
        it->second->show();
        cv::waitKey(100);
    }
}
