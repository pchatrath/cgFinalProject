#include "viewport.h"

#include "CMU462/CMU462.h"

#include <iostream>
using namespace std;

namespace CMU462 {

void ViewportImp::set_viewbox( float x, float y, float span ) {

  //cout << "HERE" << endl;
  // Task 5 (part 2): 
  // Set svg to normalized device coordinate transformation. Your input
  // arguments are defined as SVG canvans coordinates.
  this->x = x;
  this->y = y;
  this->span = span; 
  
  Matrix3x3 m;
  m(0,0) = 1.0/(2*span);
  m(1,1) = 1.0/(2*span);
  m(2,2) = 1;
  m(0,2) = 0.5+((-x) / (2*span));
  m(1,2) = 0.5+((-y) / (2*span));

  svg_2_norm = m;

  return;
}

void ViewportImp::update_viewbox( float dx, float dy, float scale ) { 
  
  this->x -= dx;
  this->y -= dy;
  this->span *= scale;
  set_viewbox( x, y, span );
}

} // namespace CMU462