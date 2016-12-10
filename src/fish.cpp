#include "fish.h"
#include "matrix3x3.h"
#include <iostream>
#include <math.h>

using namespace std;

namespace CMU462 {

Fish::Fish(SVGElement* e) {
  element = e;

  double x = rand() % 800 + 100;
  double y = rand() % 800 + 100;

  Vector2D p(x,y);
  position = p;

  heading = 2*M_PI * rand() / (RAND_MAX);

  placeFish();

  // TODO -- Set velocity randomly
  vel = .1;

  omega = 0.0;

  Vector2D x1 (0,0); corners.push_back(x1);
  Vector2D x2 (1000,0); corners.push_back(x2);
  Vector2D x3 (1000,1000); corners.push_back(x3);
  Vector2D x4 (0,1000); corners.push_back(x4);

}

void Fish::updateFish(double ts) {

  // Randomly vary velocity of fish
  vel = vel + .0005 * rand() / (RAND_MAX);
  vel = min(vel, velMax);
  vel = max(vel, velMin);

  if (fearLevel > 0.5) {
    // Fear decay
    fearLevel = fearLevel * .9995;
    position.x = position.x + 3 * velMax * cos(heading + M_PI_2);
    position.y = position.y + 3 * velMax * sin(heading + M_PI_2);
    heading = heading - 2 * torque * ts;

  } else {
    fearLevel = 0;
    position.x = position.x + vel * cos(heading + M_PI_2);
    position.y = position.y + vel * sin(heading + M_PI_2);

    heading = heading - torque * ts;
  }

  // Normalize angles
  while (heading > 2 * M_PI) heading = heading - 2 * M_PI;
  while (heading < 0) heading = heading + 2 * M_PI;

  placeFish();
}

void Fish::updateFishDistance(std::vector<Fish> otherFish) {

  fishDist.clear();
  fishHead.clear();

  for (size_t i=0; i<otherFish.size(); ++i) {
    double dist = (this->position - otherFish[i].position).norm();
    fishDist.push_back( dist );
    double head = this->heading - otherFish[i].heading;
    while (head > M_PI) head = head - M_PI;
    while (head < M_PI) head = head + M_PI;
    fishHead.push_back( this->heading - otherFish[i].heading );
  }
}

// Fish are repelled from walls
// Fish are attracted to other fish
// Fish are repelled from other fish
// Fish change orientation based on other
void Fish::updateFishForce() {

  torque = 0;
  // Attractive force
  for (size_t i=0; i<fishDist.size(); ++i) {
    if (fishDist[i] > .001 && fishDist[i] < attractionThresh) {
      torque += fishHead[i];
    }
    if (fishDist[i] > .001 && fishDist[i] < repulsionThresh) {
      torque -= 7*fishHead[i];
    }
  }

  // Arbitrary turning limits
  if (torque > .75) torque = .75;
  if (torque < -.75) torque = -.75;

  // Wall repulsion forces
  double dx = min( 1000 - position.x, position.x);
  double dy = min( 1000 - position.y, position.y);

  double wallDist = min(dx,dy);

  //vel = min(.0004 * wallDist + .0025,velMax);

  std::vector<Vector2D> intersections = getIntersections();
  
  // Stuck in corner, try to get out
  //if (dx < 100 && dy < 100) {
  //  torque = 2;
  //  return;
  //}

  std::vector<double> shortDist;
  shortDist.resize(4);
  shortDist[0]=position.y;
  shortDist[1]=1000-position.x;
  shortDist[2]=1000-position.y;
  shortDist[3]=position.x;

  int turnCt = 0;
  for (size_t i=0; i<4; ++i) {
    
    Vector2D diff = intersections[i]-position;
    double dist = diff.norm();
    Vector2D p2;
    p2.x = position.x + 1000 * cos(heading + M_PI_2);
    p2.y = position.y + 1000 * sin(heading + M_PI_2);
    double cosTheta = dot( intersections[i]-position, p2-position);
    cosTheta = cosTheta / (intersections[i]-position).norm();
    cosTheta = cosTheta / (p2-position).norm();

    if ( shortDist[i] < 200 && cosTheta > 0) {
      //cout << "SHOULD TURN" << endl;
      // TODO -- Check which way to turn
      torque = 1.75;
    } 
  }

}

void Fish::commandFish(double c_x, double c_y) {

  //cout << "X: " << c_x << " Y: " << c_y << endl;
  //cout << "X: " << position.x << " Y: " << position.y << endl;
  //cout << "Heading: " << heading << endl;

  //c_x = c_x / 960 * 1000;
  //c_y = c_y / 660 * 1000;

  position.x = c_x;
  position.y = c_y;

  heading = 2*M_PI * rand() / (RAND_MAX);

  placeFish();

  // Update Velocity based on distance from fish to mouse click
/*  Vector2D M(c_x,c_y);
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

  //cout << "DH: " << dh << endl;
  if (! (dh != dh) ) {
    heading = heading + .003 * dh;
  }*/

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

void Fish::scareFish(double x, double y) {
  Vector2D p(x,y);
  Vector2D p2 = position - p;
  if (p2.norm() < scareThresh) {
    
    fearLevel = 1 - p2.norm()*.005;

  } 
}

std::vector<Vector2D> Fish::getIntersections() {

  Vector2D p1 = position;
  Vector2D p2;

  p2.x = p1.x + 1000 * cos(heading + M_PI_2);
  p2.y = p1.y + 1000 * sin(heading + M_PI_2);

  //cout << "P1: " << p1 << " P2: << " << p2 << " C1: " << corners[0] << " C2: " << corners[1] << endl;

  std::vector<Vector2D> intersections;

  for (size_t i=0; i<4; ++i) {
    int indexMin = i;
    int indexMax = (i+1) % 4;

    Vector2D p3 = corners[indexMin];
    Vector2D p4 = corners[indexMax];

    Vector2D intersection;

    double n1 = (p1.x*p2.y-p1.y*p2.x)*(p3.x-p4.x) - (p1.x-p2.x)*(p3.x*p4.y-p3.y*p4.x);
    double d1 = (p1.x-p2.x)*(p3.y-p4.y) - (p1.y-p2.y)*(p3.x-p4.x);

    intersection.x = n1/d1;

    double n2 = (p1.x*p2.y-p1.y*p2.x)*(p3.y-p4.y) - (p1.y-p2.y)*(p3.x*p4.y-p3.y*p4.x);
    double d2 = d1;

    intersection.y = n2/d2;
    
    intersections.push_back(intersection);
  }

  return intersections;

}

}