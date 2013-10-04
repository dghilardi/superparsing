#include "tridimensionalvoxel.h"

TridimensionalVoxel::TridimensionalVoxel(vector<SuperPixel *> *spList, int firstFrameIdx, cv::Size imgDim){
    triangles=(imgDim.height>imgDim.width)?new TridimensionalObject(imgDim.height):new TridimensionalObject(imgDim.width);
    getTrianglesList(NULL, spList->at(0), *triangles, firstFrameIdx);
    for(uint i=1; i<spList->size(); ++i){
        getTrianglesList(spList->at(i-1), spList->at(i), *triangles, i+firstFrameIdx);
    }
    cv::Mat *mask = spList->back()->getMask();
    cv::Point offset = spList->back()->getTopLeftCorner();
    cv::Mat back;
    cv::copyMakeBorder(*mask, back, offset.y, 1, offset.x, 1, cv::BORDER_CONSTANT, cv::Scalar(0));
    vector<vector<p2t::Point *> > polylines;
    triangulateFace(*triangles, &back, spList->size()+firstFrameIdx, polylines);

    cv::Vec3b color;
    VisualUtils::findDistinctColor(spList->at(0)->getLabel(), 33, color);
    triangles->setColor(color[0]/255.0, color[1]/255.0, color[2]/255.0);

/*    if(spList.size()>1 && triangles.size()>0){
    for(int j=0; j<triangles.size(); ++j){
        for(int k=0; k<3; ++k){
            cout << triangles[j][k*3] << ".0f, " << triangles[j][k*3+1] << ".0f, " << triangles[j][k*3+2] << ".0f, " << endl;
        }
    }
    throw;
    }*/
}

TridimensionalVoxel::~TridimensionalVoxel(){
    delete triangles;
}

#ifdef POLY2TRI
void TridimensionalVoxel::triangulateFace(TridimensionalObject &result, cv::Mat *xorMatrix, int frameNumber, vector<vector<p2t::Point *> > &polylines)
{
    cv::Mat resized, hierarchy;
    float scaleborder = 15.0;
    vector<vector<cv::Point> > resultingContours;
    cv::resize(*xorMatrix, resized, cv::Size(), scaleborder, scaleborder, cv::INTER_NEAREST);
    cv::erode(resized, resized, cv::Mat());

    //Find contours
    cv::findContours(resized, resultingContours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    //convert contours for p2t
    for(uint i=0; i<resultingContours.size(); ++i){
        polylines.push_back(vector<p2t::Point*>());
        for(uint j=0; j<resultingContours[i].size(); ++j){
            p2t::Point *pt = new p2t::Point(resultingContours[i][j].x/scaleborder, resultingContours[i][j].y/scaleborder);
            polylines[i].push_back(pt);
        }
    }

    //compute triangles and convert p2t to cv
    vector<p2t::Triangle*> triangles;
    for(uint i=0; i<resultingContours.size(); ++i){
        if(hierarchy.at<int>(0,4*i+3)==-1){
            p2t::CDT* cdt = new p2t::CDT(polylines[i]);

            for(uint j=0; j<resultingContours.size(); ++j){
                if(hierarchy.at<int>(0,4*j+3)==i) cdt->AddHole(polylines[j]);
            }
            cdt->Triangulate();

            triangles = cdt->GetTriangles();

            for (uint k = 0; k < triangles.size(); k++) {

                p2t::Triangle& t = *triangles[k];
                Tri tri;
                for(int idx=0; idx<3; ++idx){
                    p2t::Point& point = *t.GetPoint(idx);
                    tri[3*idx] = round(point.x);
                    tri[3*idx+1] = round(point.y);
                    tri[3*idx+2] = frameNumber;
                }
                result.addTriangle(tri[0], tri[1], tri[2], tri[3], tri[4], tri[5], tri[6], tri[7], tri[8]);
            }

            delete cdt;
        }
    }
}

void TridimensionalVoxel::getTrianglesList(SuperPixel *prevSP, SuperPixel *nextSP, TridimensionalObject &result, int frameNumber){
    //initialize values
    //result.clear();
    cv::Mat *prevMask=NULL;
    cv::Point prevOffset;
    if(prevSP!=NULL){
        prevMask = prevSP->getMask();
        prevOffset = prevSP->getTopLeftCorner();
    }
    cv::Mat *nextMask = nextSP->getMask();
    cv::Point nextOffset = nextSP->getTopLeftCorner();
    cv::Mat *xorMatrix = new cv::Mat();

    //Prepare mat
    xorMatrices(prevMask, nextMask, prevOffset, nextOffset, xorMatrix);
    vector<vector<p2t::Point *> > polylines;
    triangulateFace(result, xorMatrix, frameNumber, polylines);

    //extrude
    extrudeOnePxl(&polylines, result, frameNumber);

    //clean memory
    delete xorMatrix;
    for(uint i=0; i<polylines.size(); ++i) for(uint j=0; j<polylines[i].size(); ++j) delete polylines[i][j];
}

void TridimensionalVoxel::extrudeOnePxl(vector<vector<p2t::Point *> > *contours, TridimensionalObject &triangles, int frameNumber){
    for(uint i=0; i<contours->size(); ++i){
        int elemNum = (*contours)[i].size();
        for(int j=0; j<elemNum; ++j){
            p2t::Point *actual = (*contours)[i][j];
            p2t::Point *next = (*contours)[i][(j+1)%elemNum];

            Tri triA;
            triA[0] = round(actual->x); triA[1] = round(actual->y); triA[2] = frameNumber;
            triA[3] = round(next->x);   triA[4] = round(next->y);   triA[5] = frameNumber;
            triA[6] = triA[3];     triA[7] = triA[4];     triA[8] = frameNumber+1;

            Tri triB;
            triB[0] = round(next->x);   triB[1] = round(next->y);   triB[2] = frameNumber+1;
            triB[3] = round(actual->x); triB[4] = round(actual->y); triB[5] = frameNumber+1;
            triB[6] = triB[3];     triB[7] = triB[4];     triB[8] = frameNumber;

            triangles.addTriangle(triA[0], triA[1], triA[2], triA[3], triA[4], triA[5], triA[6], triA[7], triA[8]);
            triangles.addTriangle(triB[0], triB[1], triB[2], triB[3], triB[4], triB[5], triB[6], triB[7], triB[8]);
        }
    }
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
#endif

void TridimensionalVoxel::xorMatrices(cv::Mat *mat1, cv::Mat *mat2, cv::Point mat1Offset, cv::Point mat2Offset, cv::Mat *result){
    if(mat1==NULL)
        cv::copyMakeBorder(*mat2, *result, mat2Offset.y, 1, mat2Offset.x, 1, cv::BORDER_CONSTANT, cv::Scalar(0));
    else{
        int maxWidth = max(mat1Offset.x+mat1->cols, mat2Offset.x+mat2->cols);
        int maxHeight = max(mat1Offset.y+mat1->rows, mat2Offset.y+mat2->rows);
        result->create(maxHeight+1, maxWidth+1, CV_8UC1);
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

TridimensionalObject *TridimensionalVoxel::getObject(){
    return triangles;
}

void TridimensionalVoxel::drawTriangles(vector<Tri> &triangles, cv::Mat *mask){
    const int scalef = 5;
    cv::Mat toDraw;
    mask->copyTo(toDraw);
    cv::resize(*mask, toDraw, cv::Size(), scalef, scalef, cv::INTER_NEAREST);
    int minX = toDraw.cols;
    int minY = toDraw.rows;
    for(uint i=0; i<triangles.size(); ++i){
        for(int j=0; j<3; ++j){
            int nextPoint = (j+1)%3;
            if(minX>scalef*triangles[i][j*3] && triangles[i][j*3]>=0) minX=scalef*triangles[i][j*3];
            if(minY>scalef*triangles[i][j*3+1] && triangles[i][j*3+1]>=0) minY=scalef*triangles[i][j*3+1];
            //minX = min(minX, scalef*triangles[i][j*2]);
            //minY = min(minY, scalef*triangles[i][j*2+1]);
            cv::Point pointA(scalef*triangles[i][j*3], scalef*triangles[i][j*3+1]);
            cv::Point pointB(scalef*triangles[i][nextPoint*3], scalef*triangles[i][nextPoint*3+1]);
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

string TridimensionalVoxel::type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

