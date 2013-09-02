#include "queryvideo.h"

QueryVideo::QueryVideo(string path): video(path){
    vector<IplImage *> frames;
    vector<vector<Pixel> > superPixels;

    video.getImages(frames, 25);
    segmentVideo(frames, superPixels);

    for(int i=0; i<frames.size(); ++i) delete frames[i];
    frames.clear();
}
