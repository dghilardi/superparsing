#include "supervoxel.h"

SuperVoxel::SuperVoxel(vector<Pixel> &pixelList, vector<IplImage *> &frames)
{
    map<int, vector<Pixel> > dividedPerFrame;
    for(int i=0; i<pixelList.size(); ++i){
        dividedPerFrame[pixelList[i].f].push_back(pixelList[i]);
    }

    for(map<int, vector<Pixel> >::iterator it=dividedPerFrame.begin(); it!=dividedPerFrame.end(); ++it){
        cv::Mat tmp = cv::cvarrToMat(frames[it->first]);
        superPixelsList.insert(map<int, SuperPixel>::value_type(it->first, SuperPixel(it->second, tmp)));
    }
}

const map<int, SuperPixel> *SuperVoxel::getSuperPixels(){
    return &superPixelsList;
}
