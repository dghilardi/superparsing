#include "supervoxel.h"

SuperVoxel::SuperVoxel(vector<Pixel> &pixelList, vector<cv::Mat> &frames) : label(-1)
{
    map<int, vector<Pixel> > dividedPerFrame;
    for(uint i=0; i<pixelList.size(); ++i){
        dividedPerFrame[pixelList[i].f].push_back(pixelList[i]);
    }

    firstFrame = dividedPerFrame.begin()->first;
    for(map<int, vector<Pixel> >::iterator it=dividedPerFrame.begin(); it!=dividedPerFrame.end(); ++it){
        superPixelsList.push_back(new SuperPixel(it->second, frames[it->first]));
        if(firstFrame>it->first) firstFrame = it->first;
    }
}

SuperVoxel::~SuperVoxel(){
    for(int i=0; i<superPixelsList.size(); ++i){
        delete superPixelsList[i];
    }
}

vector<SuperPixel *> *SuperVoxel::getSuperPixels(){
    return &superPixelsList;
}

int SuperVoxel::getLabel(){
#ifndef NDEBUG
    for(int i=0; i<superPixelsList.size(); ++i){
        assert(label==superPixelsList[i]->getLabel());
    }
#endif
    return label;
}

int SuperVoxel::getFirstFrameIdx(){
    return firstFrame;
}

void SuperVoxel::setLabel(int newLabel){
    label=newLabel;
    for(int i=0; i<superPixelsList.size(); ++i){
        superPixelsList[i]->setLabel(newLabel);
    }
}

void SuperVoxel::show(){
    for(int i=0; i<superPixelsList.size(); ++i){
        superPixelsList[i]->show();
        cv::waitKey(100);
    }
}

/**
 * @brief SuperVoxel::appendAdiacent Insert a supervoxel as a adiacent
 * @param toAppend SuperVoxel to insert as adiacent
 */
void SuperVoxel::appendAdiacent(SuperVoxel *toAppend){
    adiacentsSet.insert(toAppend);
}

/**
 * @brief SuperVoxel::getAdiacents return pointer to the set of adiacents
 * @return
 */
const set<SuperVoxel *> *SuperVoxel::getAdiacents(){
    return &adiacentsSet;
}

void SuperVoxel::computePerFrameNeighbour(vector<SuperVoxel *> superVoxelList, int height, int width, int nframes){
    /*map<int, vector<SuperPixel *> > dividedPerFrame;
    for(uint i=0; i<superVoxelList.size(); ++i){
        SuperVoxel *actualVox = superVoxelList[i];
        for(map<int, SuperPixel *>::iterator it=actualVox->getSuperPixels()->begin();it!=actualVox->getSuperPixels()->end(); ++it){
            dividedPerFrame[it->first].push_back(it->second);
        }
    }
    for(map<int, vector<SuperPixel *> >::iterator it=dividedPerFrame.begin(); it!=dividedPerFrame.end(); ++it){
        SuperPixel::computeAdiacents(it->second, height, width);
    }*/
    SuperVoxel**** superVoxelMap = new SuperVoxel***[nframes];
    for(int i=0; i<nframes; ++i){
        superVoxelMap[i] = new SuperVoxel**[width];
        for(int j=0; j<width; ++j){
            superVoxelMap[i][j] = new SuperVoxel*[height];
            for(int k=0; k<height; ++k) superVoxelMap[i][j][k] = NULL;
        }
    }
    for(uint i=0; i<superVoxelList.size(); ++i){
        SuperVoxel *actualVox = superVoxelList[i];
        for(int it=0; it<actualVox->getSuperPixels()->size(); ++it){
            SuperPixel *actualSP = actualVox->getSuperPixels()->at(it);
            for(uint j=0; j<actualSP->getCoordList()->size(); ++j){
                Pixel toAdd = (*(actualSP->getCoordList()))[j];
                superVoxelMap[toAdd.f][toAdd.x][toAdd.y] = actualVox;
            }
        }
    }

    for(int x=0; x<width-1; ++x){ for(int y=0; y<height-1; ++y){ for(int f=0; f<nframes; ++f){
                if(y>0 && superVoxelMap[f][x+1][y-1]!=NULL && superVoxelMap[f][x+1][y-1]!=superVoxelMap[f][x][y]){
                    superVoxelMap[f][x+1][y-1]->appendAdiacent(superVoxelMap[f][x][y]);
                    superVoxelMap[f][x][y]->appendAdiacent(superVoxelMap[f][x+1][y-1]);
                }
                for(int dx=0; dx<2; ++dx){ for(int dy=0; dy<2; ++dy){
                    if(dx+dy>0 && superVoxelMap[f][x+dx][y+dy]!=NULL && superVoxelMap[f][x+dx][y+dy]!=superVoxelMap[f][x][y]){
                        superVoxelMap[f][x+dx][y+dy]->appendAdiacent(superVoxelMap[f][x][y]);
                        superVoxelMap[f][x][y]->appendAdiacent(superVoxelMap[f][x+dx][y+dy]);
                    }
                }}
    }}}
}
