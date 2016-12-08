#include "fish.h"
#include "matrix3x3.h"
#include <iostream>
#include <math.h>

using namespace std;

namespace CMU462 {

Fish::Fish(SVGElement* e) {
  element = e;

  double x = rand() % 100 + 0;
  double y = rand() % 100 + 0;

  Vector2D p(x,y);
  position = p;

  Matrix3x3 tf;

  tf[2][0] = p.x;
  tf[2][1] = p.y;
  tf[0][0] = sx;
  tf[1][1] = sy;
  tf[2][2] = 1;

  element->transform = tf;

  vel = .1;
  omega = 0.0;
  heading = 0.0;

}

void Fish::updateFish(double ts) {

  Matrix3x3 tf;

  tf[2][0] = position.x;
  tf[2][1] = position.y;
  tf[0][0] = sx;
  tf[1][1] = sy;
  tf[2][2] = 1;

  element->transform = tf;  

  return;
}

void Fish::commandFish(double c_x, double c_y) {

  //cout << "X: " << c_x << " Y: " << c_y << endl;
  //cout << "X: " << position.x << " Y: " << position.y << endl;
  cout << "Heading: " << heading << endl;

  //c_x = c_x / 960 * 100;
  //c_y = c_y / 660 * 100;

  // Update Velocity based on distance from fish to mouse click
  Vector2D M(c_x,c_y);
  double dist = (M-position).norm();
  dist = dist * .0007;

  position.x = position.x + dist * cos(heading + M_PI_2);
  position.y = position.y + dist * sin(heading + M_PI_2);

  // Update heading

  Vector2D h(cos(heading),sin(heading));
  Vector2D v1 = M-position; 

  //cout << "DOT P " << dot(v1,h) << endl;

  double dh = dot(v1,h) / ( v1.norm() * h.norm() );
  dh = acos(dh);
  dh = dh - (3.1415/2);
  //dh = min(dh, 3.1415-dh);

  cout << "DH: " << dh << endl;
  if (! (dh != dh) ) {
    heading = heading + .003 * dh;
  }

  placeFish();

}

void Fish::placeFish() {
  // Update fish position
  Matrix3x3 T;
  T[2][0] = position.x;
  T[2][1] = position.y;
  T[0][0] = 1;
  T[1][1] = 1;
  T[2][2] = 1;

  Matrix3x3 R;    
  R[0][0] =  cos(heading + M_PI_2);
  R[1][1] =  cos(heading + M_PI_2);
  R[0][1] =  sin(heading + M_PI_2);
  R[1][0] = -sin(heading + M_PI_2);
  R[2][2] = 1;

  Matrix3x3 S;
  S[0][0] = sx;
  S[1][1] = sy;
  S[2][2] = 1;

  element->transform = T * R * S;  
}

}