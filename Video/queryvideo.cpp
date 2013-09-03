#include "queryvideo.h"

QueryVideo::QueryVideo(string path): video(path){
    vector<cv::Mat> frames;
    vector<vector<Pixel> > superPixels;

    cout << "Segmenting Super-Voxels: ";
    video.getImages(frames, 25);
    segmentVideo(frames, superPixels);
    cout << "OK\nExtracting Features: ";

    for(uint i=0; i<superPixels.size(); ++i){
        SuperVoxel *toAdd = new SuperVoxel(superPixels[i], frames);
        superVoxelsList.push_back(toAdd);
    }
    cout << "OK" << endl;
}

QueryVideo::~QueryVideo(){
    for(uint i=0; i<superVoxelsList.size(); ++i) delete superVoxelsList[i];
}

vector<SuperVoxel *> *QueryVideo::getSuperVoxelsList(){
    return &superVoxelsList;
}
