#include "queryvideo.h"

QueryVideo::QueryVideo(string path): video(path){
    vector<cv::Mat> frames;
    vector<vector<Pixel> > superPixels;

    cout << "Segmenting Super-Voxels:\t";
    cout.flush();
    video.getImages(frames, 25);
    int h=frames[0].rows;
    int w=frames[0].cols;
    segmentVideo(frames, superPixels);
    cout << "[OK]\nExtracting Features:\t\t";
    cout.flush();

    for(uint i=0; i<superPixels.size(); ++i){
        SuperVoxel *toAdd = new SuperVoxel(superPixels[i], frames);
        superVoxelsList.push_back(toAdd);
    }
    vector<SuperPixel *> allSuperPixels;
    for(uint i=0; i<superVoxelsList.size(); ++i){
        for(map<int, SuperPixel*>::iterator it=superVoxelsList[i]->getSuperPixels()->begin(); it!=superVoxelsList[i]->getSuperPixels()->end(); ++it){
            allSuperPixels.push_back(it->second);
        }
        //allSuperPixels.insert(allSuperPixels.end(), superVoxelsList[i]->getSuperPixels()->begin(), superVoxelsList[i]->getSuperPixels()->end());
    }
    SuperPixel::computeWeight(allSuperPixels);
    SuperVoxel::computePerFrameNeighbour(superVoxelsList, h, w);

    cout << "[OK]" << endl;
}

QueryVideo::~QueryVideo(){
    for(uint i=0; i<superVoxelsList.size(); ++i) delete superVoxelsList[i];
}

vector<SuperVoxel *> *QueryVideo::getSuperVoxelsList(){
    return &superVoxelsList;
}
