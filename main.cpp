#include "geolabel.h"

#define GEOLABEL_FILE "../SiftFlowDataset/SemanticLabels/spatial_envelope_256x256_static_8outdoorcategories/insidecity_art727.mat"

using namespace std;

int main(){
    GeoLabel data(GEOLABEL_FILE);
    data.viewLabeledImg();
    return 0;
}

