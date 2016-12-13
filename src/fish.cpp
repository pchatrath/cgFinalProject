#include "fish.h"
#include "matrix3x3.h"
#include <iostream>
#include <math.h>

using namespace std;

namespace CMU462 {

Fish::Fish(SVGElement* e, FishType T) {
  element = e;
  type = T;

  double x = rand() % width;
  double y = rand() % height;

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

void Fish::updateViewer(int w, int h) {
  width = w;
  height = h;
}

void Fish::updateFish(double ts) {

  // Randomly vary velocity of fish
  vel = vel + .0005 * rand() / (RAND_MAX);
  vel = min(vel, velMax);
  vel = max(vel, velMin);

/*  if (fearLevel > 0.5) {
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
  }*/

  position.x = position.x + vel * cos(heading + M_PI_2);
  position.y = position.y + vel * sin(heading + M_PI_2);

  heading = heading - torque * ts;

  // Normalize angles
  while (heading > 2 * M_PI) heading = heading - 2 * M_PI;
  while (heading < 0) heading = heading + 2 * M_PI;

  placeFish();
}

void Fish::updateFishDistance(std::vector<Fish*> otherFish) {

  fishDist.clear();
  fishHead.clear();

  for (size_t i=0; i<otherFish.size(); ++i) {
    Fish* f = otherFish[i];
    
    double dist = (this->position - f->position).norm();
    fishDist.push_back( dist );
    double head = this->heading - f->heading;
    while (head > M_PI) head = head - M_PI;
    while (head < M_PI) head = head + M_PI;
    fishHead.push_back( this->heading - f->heading );
  }
}

// Fish are repelled from walls
// Fish are attracted to other fish
// Fish are repelled from other fish
// Fish change orientation based on other
void Minnow::calculateForces(std::vector<Fish*> otherFish) {

  torque = 0;
  int nearShark = 0;
  float sharkTorque;
  // Attractive force
  for (size_t i=0; i<fishDist.size(); ++i) {
    if (fishDist[i] > .001 && fishDist[i] < attractionThresh) {
      torque += 5*fishHead[i];
    }
    if (fishDist[i] > .001 && fishDist[i] < repulsionThresh) {
      torque -= 7*fishHead[i];
    }
    // Minnors strongly repel turtles
    if (otherFish[i]->type == TURTLE && fishDist[i] < turtleThresh) {
      torque -= 150*( M_PI_2 - fishHead[i]);
    }

    if (otherFish[i]->type == SHARK && fishDist[i] < sharkThresh) {
      // Fish should avoid sharks at all costs
      torque = 0;
      float goalHeading = getGoalHeading(otherFish[i]);
      sharkTorque = 250 * goalHeading;
      //cout << sharkTorque << endl;
      nearShark = 1;
    }
  }

  if (nearShark) {
    torque = sharkTorque;
  }

  // Arbitrary turning limits
  if (torque > 2) torque = 2;
  if (torque < -2) torque = -2;

}

float Minnow::getGoalHeading(Fish* otherFish) {
  // Calculate goal vector 
  Vector2D goalVector = position - otherFish->position;
  double goalHeading = atan2(goalVector.y, goalVector.x);
  while (goalHeading > M_PI) goalHeading = goalHeading - M_PI;
  while (goalHeading < M_PI) goalHeading = goalHeading + M_PI;
  float torque = heading - (float) goalHeading;
  return torque;
}

// Useful for testing
void Fish::commandFish(double c_x, double c_y) {

  position.x = c_x;
  position.y = c_y;

  heading = 2*M_PI * rand() / (RAND_MAX);

  placeFish();

}

void Fish::placeFish() {

  // Check position limits
  // TODO -- Add buffer 
  if (position.x > (width + buffer)) {
    position.x = position.x - (width + buffer);
  }
  if (position.x < (0-buffer)) {
    position.x = position.x + (width+buffer);
  }
  if (position.y > (height + buffer)) {
    position.y = position.y - (height + buffer);
  }
  if (position.y < (0-buffer)) {
    position.y = position.y + (height + buffer);
  }  

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

void Minnow::scareFish(double x, double y) {
  Vector2D p(x,y);
  Vector2D p2 = position - p;
  if (p2.norm() < scareThresh) {
    
    fearLevel = 1 - p2.norm()*.005;

  } 
}


void Turtle::calculateForces() {
  
  // Turtles just roam
  float dTorque = .01;
  torque = torque - dTorque * rand() / (RAND_MAX) + dTorque/2.0;

  // With small random probability, 0 torque
  double p = 1.0 * rand() / (RAND_MAX);
  if (p < .001) {
    torque = 0;
  }

}

void Shark::calculateForces() {
  
  // Right now, sharks just roam
  float dTorque = .02;
  torque = torque - dTorque * rand() / (RAND_MAX) + dTorque/2.0;

  // With small random probability, 0 torque
  double p = 1.0 * rand() / (RAND_MAX);
  if (p < .001) {
    torque = 0;
  }

  // Useful to debug fish movements 
  position.x = 300;
  position.y = 300;
  heading = 0;

}

std::vector<Vector2D> Fish::getIntersections() {

  Vector2D p1 = position;
  Vector2D p2;

  p2.x = p1.x + 1000 * cos(heading + M_PI_2);
  p2.y = p1.y + 1000 * sin(heading + M_PI_2);

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