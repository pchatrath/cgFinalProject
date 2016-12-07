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

  position.x = position.x + vel * ts * cos(heading);
  position.y = position.y + vel * ts * sin(heading);

  Matrix3x3 tf;

  tf[2][0] = position.x;
  tf[2][1] = position.y;
  tf[0][0] = sx;
  tf[1][1] = sy;
  tf[2][2] = 1;

  element->transform = tf;  

  return;
}

}