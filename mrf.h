#ifndef MRF_H
#define MRF_H

#include "debugHeader.h"

#include <dai/var.h>
#include <dai/factor.h>
#include <vector>

#include "superpixel.h"
#include "geolabel.h"

using namespace std;
class MRF
{
public:
    MRF();
    static void computeMRF(vector<SuperPixel *> &superPixelList);
};

#endif // MRF_H
