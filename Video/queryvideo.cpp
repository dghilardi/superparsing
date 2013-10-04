#include "queryvideo.h"

QueryVideo::QueryVideo(string path): video(path){
    vector<cv::Mat> frames;
    vector<vector<Pixel> > superPixels;

    cout << "Segmenting Super-Voxels:\t";
    cout.flush();
    video.getImages(frames, 75);
    vidSize = frames[0].size();
    int h=frames[0].rows;
    int w=frames[0].cols;
    segmentVideo(frames, superPixels);
    cout << "[OK]\nFounded " << superPixels.size() << " Super-Voxels"<<endl;
    cout << "Extracting Features:\t\t";
    cout.flush();

    for(uint i=0; i<superPixels.size(); ++i){
        SuperVoxel *toAdd = new SuperVoxel(superPixels[i], frames);
        superVoxelsList.push_back(toAdd);
    }
    vector<SuperPixel *> allSuperPixels;
    for(uint i=0; i<superVoxelsList.size(); ++i){
        for(int it=0; it<superVoxelsList[i]->getSuperPixels()->size(); ++it){
            allSuperPixels.push_back(superVoxelsList[i]->getSuperPixels()->at(it));
        }
        //allSuperPixels.insert(allSuperPixels.end(), superVoxelsList[i]->getSuperPixels()->begin(), superVoxelsList[i]->getSuperPixels()->end());
    }
    SuperPixel::computeWeight(allSuperPixels);
    SuperVoxel::computePerFrameNeighbour(superVoxelsList, h, w, frames.size());
    cout << "[OK]" << endl;
    //showTridimensionalVoxels();
}

QueryVideo::~QueryVideo(){
    for(uint i=0; i<superVoxelsList.size(); ++i) delete superVoxelsList[i];
    for(uint i=0; i<glVoxelsList.size(); ++i) delete glVoxelsList[i];
    for(uint i=0; i<geoEngines.size(); ++i) delete geoEngines[i];
}

vector<SuperVoxel *> *QueryVideo::getSuperVoxelsList(){
    return &superVoxelsList;
}

void QueryVideo::showSegmentation(vector<vector<Pixel> > &coordList, int height, int width, int nframes){
    vector<cv::Mat> matList;
    for(int i=0; i<nframes; ++i) matList.push_back(cv::Mat(height, width, CV_8UC1, cv::Scalar(0)));
    double delta=255.0/coordList.size();
    for(int i=0; i<coordList.size(); ++i){ for(int j=0; j<coordList[i].size(); ++j){
            Pixel actual = coordList[i][j];
            matList[actual.f].at<uchar>(actual.y, actual.x) = (uchar)i*delta;
    }}
    cvNamedWindow("Video Segmentation", CV_WINDOW_AUTOSIZE);
    for(int i=0; i<nframes; ++i){
        cv::imshow("Video Segmentation", matList[i]);
        cv::waitKey(300);
    }
}

vector<GeometryEngine *> *QueryVideo::showTridimensionalVoxels(){
    for(int i=0; i<superVoxelsList.size(); ++i){
        glVoxelsList.push_back(new TridimensionalVoxel(superVoxelsList[i]->getSuperPixels(), superVoxelsList[i]->getFirstFrameIdx(), vidSize));
        PrintUtils::printPercentage(i, superVoxelsList.size());
    }
    for(int i=0; i<glVoxelsList.size(); ++i){
        geoEngines.push_back(new GeometryEngine(glVoxelsList[i]->getObject()));
    }
    return &geoEngines;
}
