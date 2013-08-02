#include "superpixel.h"

SuperPixel::SuperPixel(vector<Pixel> &list, cv::Mat &srcImg): maskFeature(0){
    //Compute the bounding box of the superpixel
    int minX = list[0].x, minY = list[0].y;
    int maxX = minX, maxY = minY;
    for(uint i=1; i<list.size(); ++i){
        if(list[i].x>maxX) maxX = list[i].x;
        if(list[i].x<minX) minX = list[i].x;
        if(list[i].y>maxY) maxY = list[i].y;
        if(list[i].y<minY) minY = list[i].y;
    }

    //cout << maxX-minX+1 << "x" << maxY-minY+1 << endl;
    superPixel = new cv::Mat(maxX-minX+1, maxY-minY+1, CV_8UC3, cv::Scalar(0));
    mask = new cv::Mat(maxX-minX+1, maxY-minY+1, CV_8UC1, cv::Scalar(0));

    for(uint i=0; i<list.size(); ++i){
        int x = list[i].x;
        int y = list[i].y;
        //cout << y-minY << ", " << x-minX << "["<<maxY-minY+1<<"-"<<maxX-minX+1 <<"]"<<endl;
        cv::Point3_<uchar> *src = srcImg.ptr<cv::Point3_<uchar> >(y,x);
        cv::Point3_<uchar> *dst = superPixel->ptr<cv::Point3_<uchar> >(y-minY,x-minX);
        //dst->x = src->x; //Linee necessarie ma danno problemi nell'esecuzione (pare scrivano in zone di memoria non destinate a loro)
        //dst->y = src->y;
        //dst->z = src->z;
        //mask->at<uchar>(y-minY, x-minX) = 255;
    }

    computeMaskFeature(list, minX, minY, maxX, maxY);
    relHeightFeature = (srcImg.rows-minY)/(double)srcImg.rows;
}

SuperPixel::~SuperPixel(){
    delete superPixel;
    delete mask;
}

/**
 * @brief SuperPixel::computeShapeFeature Get an 8x8 mask of the shape of the superpixel (64bit)
 */
void SuperPixel::computeMaskFeature(vector<Pixel> &pixelList, int minX, int minY, int maxX, int maxY){
    //Parameters for the computation of the mask-feature
    int dimX = maxX-minX;
    int dimY = maxY-minY;
    double divisorX = dimX/(double)8, divisorY=dimY/(double)8;
    int bucket[8][8] = {0};

    //Fill buckets to check if a cell of the feature will be considered set or unset
    for(uint i=0; i<pixelList.size(); ++i){
        int x = pixelList[i].x;
        int y = pixelList[i].y;
        bucket[(int)((x-minX)/divisorX)][(int)((y-minY)/divisorY)]++;
    }

    for(int i=0; i<8; ++i){ for(int j=0; j<8; ++j){
            int actualXindex = (int)(i*divisorX);
            int successorXindex = (int)((i+1)*divisorX);
            if(((int)(successorXindex/divisorX))!=i+1) ++successorXindex;
            if(((int)(actualXindex/divisorX))!=i) ++actualXindex;

            int actualYindex = (int)(j*divisorY);
            int successorYindex = (int)((j+1)*divisorY);
            if(((int)(successorYindex/divisorY))!=j+1) successorYindex++;
            if(((int)(actualYindex/divisorY))!=j) actualYindex++;

            int bucketSize = (successorXindex-actualXindex)*(successorYindex-actualYindex);

            if(bucket[i][j]>bucketSize/2){
                maskFeature ^= ((uint64)1 << (8*i+j));
                if(8*i+j>63) throw;
            }
            //bitset<64> bits(maskFeature);
            //cout << bits << endl;
    }}

    //cout << "Mask: " << maskFeature << endl;

}

uint64 SuperPixel::getMaskFeature(){
    return maskFeature;
}

/**
 * @brief SuperPixel::getMaskDistance Compute the distance between two 64-bit masks
 * @param otherMask the mask to confront with
 * @return The number of different cells in the mask
 */
int SuperPixel::getMaskDistance(uint64 otherMask){
    int distance = 0;
    uint64 axorb = otherMask^maskFeature;
    while(axorb){
      distance++;
      axorb &= axorb-1;
    }
    return distance;
}
