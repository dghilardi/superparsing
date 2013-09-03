#include "queryvideo.h"

QueryVideo::QueryVideo(string path): video(path){
    vector<cv::Mat> frames;
    vector<vector<Pixel> > superPixels;

    video.getImages(frames, 25);
    segmentVideo(frames, superPixels);
}
