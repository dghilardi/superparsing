#ifndef SEGMENT_VID
#define SEGMENT_VID

#include <cstdlib>
#include "image.h"
#include "misc.h"
#include "filter.h"
#include "segment-graph.h"
#include <map>
#include <opencv2/highgui/highgui.hpp>
using namespace std;

static inline float diff2(vector<image<float> *> &r, vector<image<float> *> &g, vector<image<float> *> &b,
             int x1, int y1, int f1, int x2, int y2, int f2) {
    double sum = square(imRef(r[f1], x1, y1)-imRef(r[f2], x2, y2)) +
            square(imRef(g[f1], x1, y1)-imRef(g[f2], x2, y2)) +
            square(imRef(b[f1], x1, y1)-imRef(b[f2], x2, y2));
    return sqrt(sum);
}

/*
 * Segment an image
 *
 * Returns a color image representing the segmentation.
 *
 * im: image to segment.
 * sigma: to smooth the image.
 * c: constant for treshold function.
 * min_size: minimum component size (enforced by post-processing stage).
 * num_ccs: number of connected components in the segmentation.
 */
void segment_imseq(vector<image<rgb> *> im, float sigma, float c, int min_size,
			  int *num_ccs,vector<vector<Pixel> > &resultBuffer) {
  int width = im[0]->width();
  int height = im[0]->height();
  int nframes = im.size();

  vector<image<float> *> r, g, b;
  for(int i=0; i<nframes; ++i){
      r.push_back(new image<float>(width, height));
      g.push_back(new image<float>(width, height));
      b.push_back(new image<float>(width, height));
  }

  // smooth each color channel
  for(int f=0; f<nframes; ++f){
      for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            imRef(r[f], x, y) = imRef(im[f], x, y).r;
            imRef(g[f], x, y) = imRef(im[f], x, y).g;
            imRef(b[f], x, y) = imRef(im[f], x, y).b;
        }
      }
  }
  vector<image<float> *> smooth_r, smooth_g, smooth_b;
  for(int f=0; f<nframes; ++f){
      smooth_r.push_back(smooth(r[f], sigma));
      smooth_g.push_back(smooth(g[f], sigma));
      smooth_b.push_back(smooth(b[f], sigma));

      delete r[f];
      delete g[f];
      delete b[f];
  }
  // build graph
  const int size = width*height;
  edge *edges = new edge[nframes*size*13];
  int num = 0;
  for(int f=0; f<nframes; ++f){ for(int y=0; y<height; ++y){ for(int x=0; x<width; ++x){
     int actualIndex = f*size + y*width +x;
     int dy=1, dx, df;
     if(y+dy<height) for(df=-1; df<2; ++df){ if(f+df<nframes && f+df>=0) for(dx=-1; dx<2; ++dx){ if(x+dx<width && x+dx>=0){
         edges[num].a = actualIndex;
         edges[num].b = (f+df)*size+(y+dy)*width+x+dx;
         edges[num].w = diff2(smooth_r, smooth_g, smooth_b, x, y, f, x+dx, y+dy, f+df);
         ++num;
     }}}

     dy=0;
     dx=1;
     if(x+dx<width) for(df=-1; df<2; ++df){ if(f+df<nframes && f+df>=0){
         edges[num].a = actualIndex;
         edges[num].b = (f+df)*size+(y+dy)*width+x+dx;
         edges[num].w = diff2(smooth_r, smooth_g, smooth_b, x, y, f, x+dx, y+dy, f+df);
         ++num;
     }}

     dx=0;
     dy=0;
     df=1;
     if(f+df<nframes){
         edges[num].a = actualIndex;
         edges[num].b = (f+df)*size+(y+dy)*width+x+dx;
         edges[num].w = diff2(smooth_r, smooth_g, smooth_b, x, y, f, x+dx, y+dy, f+df);
         ++num;
     }
  }}}

  for(int i=0; i<nframes; ++i){
      delete smooth_r[i];
      delete smooth_g[i];
      delete smooth_b[i];
  }

  // segment
  universe *u = segment_graph(nframes*width*height, num, edges, c);

  // post process small components
  for (int i = 0; i < num; i++) {
    int a = u->find(edges[i].a);
    int b = u->find(edges[i].b);
    if ((a != b) && ((u->size(a) < min_size) || (u->size(b) < min_size)))
      u->join(a, b);
  }
  delete [] edges;
  if(num_ccs)*num_ccs = u->num_sets();


  //Modification: output each segment as a vector of pixels.
  map<int,vector<Pixel> > results; 

  for(int f=0; f<nframes; ++f){
      for (int y = 0; y < height; y++) {
          for (int x = 0; x < width; x++) {
              int comp = u->find(f*size+y * width + x);
              Pixel pix;
              pix.x = x;
              pix.y = y;
              pix.f = f;
              if(results.find(comp)!=results.end()){
                  results[comp].push_back(pix);
              }else{
                  vector<Pixel> vec;
                  vec.push_back(pix);
                  results.insert(std::pair<int, vector<Pixel> >(comp,vec));
              }
          }
      }
  }
  delete u;
  map<int,vector<Pixel> >::iterator it;
    for (it=results.begin() ; it != results.end(); it++){
        resultBuffer.push_back(it->second);        
    }
}

#endif
