#include "tridimensionalvoxel.h"

TridimensionalVoxel::TridimensionalVoxel(vector<SuperPixel *> &spList){
    vector<cv::Vec6f> triangles;
    getTrianglesList(NULL, spList[0], triangles);
    for(uint i=1; i<spList.size(); ++i){
        for(int j=0; j<triangles.size(); ++j){
            for(int k=0; k<3; ++k){
                cout << triangles[j][k*2] << ".0f, " << triangles[j][k*2+1] << ".0f, " << i-1 << ".0f, " << endl;
            }
        }
        getTrianglesList(spList[i-1], spList[i], triangles);
    }
}

#ifdef POLY2TRI
void TridimensionalVoxel::getTrianglesList(SuperPixel *prevSP, SuperPixel *nextSP, vector<cv::Vec6f> &result){
    vector<Triangle*> triangles;
    //vector< vector<Point*> > polylines;
    //vector<Point*> polyline;
}
#else
void TridimensionalVoxel::getTrianglesList(SuperPixel *prevSP, SuperPixel *nextSP, vector<cv::Vec6f> &result){
    result.clear();
    cv::Mat *prevMask=NULL;
    cv::Point prevOffset;
    if(prevSP!=NULL){
        prevMask = prevSP->getMask();
        prevOffset = prevSP->getTopLeftCorner();
    }
    cv::Mat *nextMask = nextSP->getMask();
    cv::Point nextOffset = nextSP->getTopLeftCorner();
    cv::Mat *xorMatrix=new cv::Mat();
    xorMatrices(prevMask, nextMask, prevOffset, nextOffset, xorMatrix);

    vector<vector<cv::Point> > resultingContours;
    cv::findContours(xorMatrix->clone(), resultingContours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    for(int i=0; i<resultingContours.size(); ++i) for(int j=0; j<resultingContours[i].size(); ++j) cout << (j==0?"\n->":"") << resultingContours[i][j];
    cout << endl;

    int maxHeight, maxWidth;
    if(prevMask!=NULL){
        maxWidth = max(prevOffset.x+prevMask->cols, nextOffset.x+nextMask->cols);
        maxHeight = max(prevOffset.y+prevMask->rows, nextOffset.y+nextMask->rows);
    }else{
        maxWidth = nextOffset.x+nextMask->cols;
        maxHeight = nextOffset.y+nextMask->rows;
    }

    cv::Rect rect(0,0,maxWidth+1, maxHeight+1);
    cv::Subdiv2D subdiv(rect);
    for(int x=0; x<=maxWidth; ++x){
        for(int y=0; y<=maxHeight; ++y){
            if(isVertex(x,y,xorMatrix)){
                cv::Point2f fp((float)x, (float)y);
                subdiv.insert(fp);
            }
        }
    }
    vector<cv::Vec6f> allTriangles;
    subdiv.getTriangleList(allTriangles);
    cleanTriangles(allTriangles, result, *xorMatrix);

    //cout << "Triangles: ";
    //for(int i=0; i<result.size(); ++i) cout << result[i] << " ";
    //cout << endl << endl;
/*
    if(prevMask!=NULL){
        cvNamedWindow("MAT1", CV_WINDOW_AUTOSIZE);
        cv::imshow("MAT1", *prevMask);
        cvNamedWindow("MAT2", CV_WINDOW_AUTOSIZE);
        cv::imshow("MAT2", *nextMask);
        cvNamedWindow("XOR", CV_WINDOW_AUTOSIZE);
        cv::imshow("XOR", *xorMatrix);
        cv::waitKey();
    }*/
    if(result.size()>100){
        drawTriangles(result, xorMatrix);
        cv::waitKey();
    }

    delete xorMatrix;
}
#endif

/**
 * @brief isVertex The position indicated by coordinates x and y represent a vertex iff one of the four adiacent pixels is set (unset) and all the others unset (set)
 * @param x
 * @param y
 * @param mask
 * @return
 */
bool TridimensionalVoxel::isVertex(int x, int y, cv::Mat *mask){
    assert(mask->type()==CV_8UC1);
    int nSet = 0, nNotSet = 0, nx, ny;
    for(int dx=-1; dx<1; ++dx){
        for(int dy=-1; dy<1; ++dy){
            nx = x+dx;
            ny = y+dy;
            if(nx<0 || nx>=mask->cols) nNotSet++;
            else if(ny<0 || ny>=mask->rows) nNotSet++;
            else if(mask->at<uchar>(ny, nx) == 0) nNotSet++;
            else nSet++;
        }
    }
    assert(nSet+nNotSet==4);
    if(nSet>2 && nNotSet>0) return true;
    if(nNotSet>2 && nSet>0) return true;
    if(nSet==2 && mask->at<uchar>(y-1,x)==mask->at<uchar>(y,x-1)) return true;
    return false;

}

/**
 * @brief cleanTriangles Remove triangles out of the region
 * @param allTriangles
 * @param result
 * @param mask
 */
void TridimensionalVoxel::cleanTriangles(vector<cv::Vec6f> &allTriangles, vector<cv::Vec6f> &result, cv::Mat &mask){
    for(int i=0; i<allTriangles.size(); ++i){
        bool notIn = false, isSet=false;
        int mx=0, my=0;
        for(int j=0; j<3; ++j){
            if(allTriangles[i][2*j]>mask.cols || allTriangles[i][2*j]<0) notIn = true;
            if(allTriangles[i][2*j+1]>mask.rows || allTriangles[i][2*j+1]<0) notIn = true;
            mx+=allTriangles[i][2*j];
            my+=allTriangles[i][2*j+1];
        }
        mx=mx/3;
        my=my/3;
        if(mx>=0 && mx<mask.cols && my>=0 && my<=mask.rows && mask.at<uchar>(my,mx)!=0) isSet=true;

        if(!notIn && isSet)
            result.push_back(allTriangles[i]);
    }
}

void TridimensionalVoxel::xorMatrices(cv::Mat *mat1, cv::Mat *mat2, cv::Point mat1Offset, cv::Point mat2Offset, cv::Mat *result){
    if(mat1==NULL)
        cv::copyMakeBorder(*mat2, *result, mat2Offset.y, 0, mat2Offset.x, 0, cv::BORDER_CONSTANT, cv::Scalar(0));
    else{
        int maxWidth = max(mat1Offset.x+mat1->cols, mat2Offset.x+mat2->cols);
        int maxHeight = max(mat1Offset.y+mat1->rows, mat2Offset.y+mat2->rows);
        result->create(maxHeight, maxWidth, CV_8UC1);
        result->setTo(cv::Scalar(0));
        for(int x=0; x<maxWidth; ++x){
            for(int y=0; y<maxHeight; ++y){
                bool mat1pxl, mat2pxl;
                mat1pxl = x>=mat1Offset.x && x-mat1Offset.x<mat1->cols && y>= mat1Offset.y && y-mat1Offset.y < mat1->rows && mat1->at<uchar>(y-mat1Offset.y,x-mat1Offset.x)!=0;
                mat2pxl = x>=mat2Offset.x && x-mat2Offset.x<mat2->cols && y>= mat2Offset.y && y-mat2Offset.y < mat2->rows && mat2->at<uchar>(y-mat2Offset.y,x-mat2Offset.x)!=0;
                result->at<uchar>(y,x) = (mat1pxl!=mat2pxl?255:0);
            }
        }
    }

}

void TridimensionalVoxel::drawTriangles(vector<cv::Vec6f> &triangles, cv::Mat *mask){
    const int scalef = 12;
    cv::Mat toDraw;
    mask->copyTo(toDraw);
    cv::resize(*mask, toDraw, cv::Size(), scalef, scalef, cv::INTER_NEAREST);
    int minX = toDraw.cols;
    int minY = toDraw.rows;
    for(uint i=0; i<triangles.size(); ++i){
        for(int j=0; j<3; ++j){
            int nextPoint = (j+1)%3;
            if(minX>scalef*triangles[i][j*2] && triangles[i][j*2]>=0) minX=scalef*triangles[i][j*2];
            if(minY>scalef*triangles[i][j*2+1] && triangles[i][j*2+1]>=0) minY=scalef*triangles[i][j*2+1];
            //minX = min(minX, scalef*triangles[i][j*2]);
            //minY = min(minY, scalef*triangles[i][j*2+1]);
            cv::Point pointA(scalef*triangles[i][j*2], scalef*triangles[i][j*2+1]);
            cv::Point pointB(scalef*triangles[i][nextPoint*2], scalef*triangles[i][nextPoint*2+1]);
            cv::circle(toDraw, pointA, scalef/4, cv::Scalar(70), -1);
            cv::circle(toDraw, pointB, scalef/4, cv::Scalar(70), -1);
            cv::line(toDraw, pointA, pointB, cv::Scalar(128));
        }
    }
    cv::Rect roi(minX, minY, toDraw.cols-minX, toDraw.rows-minY);
    cout << roi << endl;
    cv::Mat toShow = toDraw(roi);
    cvNamedWindow("TRIANGLES", CV_WINDOW_AUTOSIZE);
    cv::imshow("TRIANGLES", toShow);
}
