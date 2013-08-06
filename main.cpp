#include "retrimage.h"
#include "queryimage.h"
#include "retrievalset.h"
#include "opencv2/highgui/highgui.hpp"

#define IMG_NAME "street_par33"

using namespace std;

int main(){
    vector<string> retrievalSet;
    retrievalSet.push_back("street_par36");
    retrievalSet.push_back("street_par42");
    retrievalSet.push_back("street_par49");
    QueryImage queryImg(IMG_NAME);
    RetrievalSet db(retrievalSet);
    db.LabelImg(queryImg);
    cv::waitKey();
    return 0;
}

