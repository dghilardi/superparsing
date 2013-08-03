#include "superpixel.h"
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv/cv.hpp>
#include <math.h>

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

/**
 * @brief SuperPixel::computeSiftFeature Calcola la SIFT feature del superpixel.
 * @param list: lista di punti del superpixel
 * @param srcImg: immagine sorgente
 * @param minX: estremo del superpixel
 * @param minY: estremo del superpixel
 * @param maxX: estremo del superpixel
 * @param maxY: estremo del superpixel
 * @return puntatore all'array rappresentante l'istogramma degli orientamenti dei keypoints
 */
int* SuperPixel::computeSiftFeature(vector<Pixel> &list, cv::Mat &srcImg, int minX, int minY, int maxX, int maxY){
    //Ritaglio il superpixel dall'immagine originale
    superPixel = new cv::Mat(maxX-minX+1, maxY-minY+1, CV_8UC3, cv::Scalar(0));
    for(uint i=0; i<list.size(); ++i){
        int x = list[i].x;
        int y = list[i].y;
        //cout << y-minY << ", " << x-minX << "["<<maxY-minY+1<<"-"<<maxX-minX+1 <<"]"<<endl;
        cv::Point3_<uchar> *src = srcImg.ptr<cv::Point3_<uchar> >(y,x);
        cv::Point3_<uchar> *dst = superPixel->ptr<cv::Point3_<uchar> >(y-minY,x-minX);
        dst->x = src->x; //Linee necessarie ma danno problemi nell'esecuzione (pare scrivano in zone di memoria non destinate a loro)
        dst->y = src->y;
        dst->z = src->z;
    }

    //Individuo i Keypoints del superPixel usando SIFT
    cv::SiftFeatureDetector detector;
    std::vector<cv::KeyPoint> keypoints;
    detector.detect(*superPixel,keypoints);

    /*Calcolo istogramma quantizzato degli orientamenti dei gradienti.
     *Considero come istogramma un vettore di 100 posizioni, ognuna delle quali
     *rappresenta un intervallo di 36 gradi e contiene il numero di keypoints
     *aventi orientamento appartenente a tale intervallo
     */
    int* hist;
    hist = (int*) calloc(100, sizeof(int));

    for(int i=0; i<100; i++){
        hist[i]=0;
    }
    for(int i=0; i<keypoints.size(); i++){
        int bucket = std::floor(keypoints[i].angle/(0.2*CV_PI));
        hist[bucket]++;
    }

    return hist;
}

cv::Mat* SuperPixel::computeColorFeature(vector<Pixel> &list, cv::Mat &srcImg, int minX, int minY, int maxX, int maxY){
    //Ritaglio il superpixel dall'immagine originale
    superPixel = new cv::Mat(maxX-minX+1, maxY-minY+1, CV_8UC3, cv::Scalar(0));
    for(uint i=0; i<list.size(); ++i){
        int x = list[i].x;
        int y = list[i].y;
        //cout << y-minY << ", " << x-minX << "["<<maxY-minY+1<<"-"<<maxX-minX+1 <<"]"<<endl;
        cv::Point3_<uchar> *src = srcImg.ptr<cv::Point3_<uchar> >(y,x);
        cv::Point3_<uchar> *dst = superPixel->ptr<cv::Point3_<uchar> >(y-minY,x-minX);
        dst->x = src->x; //Linee necessarie ma danno problemi nell'esecuzione (pare scrivano in zone di memoria non destinate a loro)
        dst->y = src->y;
        dst->z = src->z;
    }

    //Separo il superpixel in 3 canali RGB
    vector<cv::Mat> rgb_planes;
    cv::split(*superPixel, rgb_planes);
    //Stabilisco il numero di bins e il range di valori
    int histSize = 11;
    float range[] = {0,256};
    const float* histRange = {range};
    //Imposto flag per avere una distribuzione uniforme dei bins e affiché gli istogrammi siano inizialmente vuoti
    bool uniform = true;
    bool accumulate = false;
    //Istanzio i 3 istogrammi (1 per canale)
    cv::Mat r_hist, g_hist, b_hist;
    //Calcolo i 3 istogrammi
    calcHist( &rgb_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &rgb_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &rgb_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
    //Creo un array di array contenente i tre istogrammi e lo ritorno
    cv::Mat* hists;
    hists = (cv::Mat*) calloc(3, sizeof(cv::Mat));
    hists[0] = r_hist;
    hists[1] = g_hist;
    hists[2] = b_hist;

    return hists;
}
