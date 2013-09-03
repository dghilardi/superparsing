#include "queryvideo.h"

QueryVideo::QueryVideo(string path): video(path){
    vector<cv::Mat> frames;
    vector<vector<Pixel> > superPixels;

    video.getImages(frames, 25);
    segmentVideo(frames, superPixels);

    for(uint i=0; i<superPixels.size(); ++i){
        SuperVoxel *toAdd = new SuperVoxel(superPixels[i], frames);
        superVoxelsList.push_back(toAdd);
    }
    superVoxelsList[13]->show();
}

QueryVideo::~QueryVideo(){
    for(uint i=0; i<superVoxelsList.size(); ++i) delete superVoxelsList[i];
}
