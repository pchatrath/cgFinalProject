#ifndef CMU462_FISH_H
#define CMU462_FISH_H

#include "vector2D.h"
#include "svg.h"

namespace CMU462 {

// Required to avoid circular dependancies
class SVGElement;

typedef enum e_FishType {
  FISH = 0,
  MINNOW,
  SHARK,
  TURTLE
} FishType;

class Fish {
  public:
    Fish( SVGElement* e, FishType t);
    virtual ~Fish() { }

    SVGElement* element; // SVG Object associated with fish
    FishType type;

    Vector2D position; // Fish position
    double heading; // Fish orientation

    // Consistent for all fish

    // Update distance and heading vectors
    // Between this fish and other given fish
    void updateFishDistance(std::vector<Fish*> otherFish);
    // Update the location (TF) of a fish 
    void placeFish(); 
    // Place a fish at a new C_X, C_Y with a new heading
    void commandFish(double c_x, double c_y);
    // Perform timestep of one fish
    void updateFish(double ts);

    // Updates the forces on a given fish
    //virtual void updateFishForce(std::vector<Fish*> otherFish) = 0;

    int width = 960;
    int height = 640;
    int buffer = 50; // Rollover buffer

    void updateViewer(int w, int h);

    double fearLevel = 0;

  protected:

    // Calculate intersections to four walls of fishtank
    std::vector<Vector2D> getIntersections();
    std::vector<Vector2D> corners;

    double force = 0;
    double torque = 0;

    double vel; // Linear velocity

    double omega; // Angular velocity

    // List to store distances and headings between fish
    std::vector<double> fishDist;
    std::vector<double> fishHead;

    // Constant scale factors
    double sx;
    double sy;

    const double attractionThresh = 60;
    const double repulsionThresh = 35;
    const double scareThresh = 100;
    const double turtleThresh = 100;
    const double sharkThresh = 100;

    double velMax;
    double velMin;

};

class Minnow: public Fish {
  public:
    Minnow(SVGElement* e, FishType t = MINNOW): Fish(e, t) {
        sx = 0.03;
        sy = 0.03;
        vel = (velMin + velMax)/2;
        velMax = 5 / 35.0;
        velMin = 2.5 / 35.0;
    }
    ~Minnow() {};

    void calculateForces(std::vector<Fish*> otherFish);
    
    void scareFish(double x, double y);

    float getGoalHeading(Vector2D otherPosition);

    Vector2D scarePt;
    double scareDistThresh = 150;
    float scareHeading;

};

class Shark: public Fish {
  public:
    Shark(SVGElement* e, FishType t = SHARK): Fish(e, t) {
        sx = 0.02;
        sy = 0.02;
        velMax = 5 / 25.0;
        velMin = 2.5 / 25.0;
        vel = (velMin + velMax)/2;
    }
    ~Shark() {};

    void calculateForces();

};


class Turtle: public Fish {
  public:
    Turtle(SVGElement* e, FishType t = TURTLE): Fish(e, t) {
        sx = 0.04;
        sy = 0.04;
        velMax = 2 / 45.0;
        velMin = 1 / 45.0;
        vel = (velMin + velMax)/2;
    }
    ~Turtle() {};

    void calculateForces();
};

/*class Fish {
  public:

    // Does this have to be a list of elements depending on implementation?
    SVGElement* element; // SVG Object associated with fish

    // Constructor should take in element of fish
    // Should position be set here or passed in???
    Fish( SVGElement* e );

    // Do we want to pass in timestep
    // Or make it a global variable?
    void updateFish(double ts);

    void commandFish(double c_x, double c_y);

    // Update distance and heading vectors
    // Between this fish and other given fish
    void updateFishDistance(std::vector<Fish> otherFish);

    // Updates the forces on a given fish
    void updateFishForce();

    // If fish within scare 
    void scareFish(double x, double y);

    // Public so can be accessed by other fish
    Vector2D position; // Fish position
    double heading; // Fish orientation

  private:

    double force;
    double torque;

    double vel = 0.1; // Linear velocity
    double velMax = 0.15;
    double velMin = 0.05;

    double fearLevel = 0;

    double omega; // Angular velocity

    // List to store distances and headings between fish
    // Updated every timestep
    std::vector<double> fishDist;
    std::vector<double> fishHead;

    // Constant scale factors
    const double sx = 0.05;
    const double sy = 0.05;

    const double attractionThresh = 50;
    const double repulsionThresh = 25;
    const double scareThresh = 100;

    void placeFish();

    std::vector<Vector2D> getIntersections();
    std::vector<Vector2D> corners;
};
*/

} // namespace CMU462

#endif // CMU462_FISH_H