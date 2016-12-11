#ifndef CMU462_FISH_H
#define CMU462_FISH_H

#include "vector2D.h"
#include "svg.h"

namespace CMU462 {

// Required to avoid circular dependancies
class SVGElement;

typedef enum e_FishType {
  NONE = 0,
  MINNOW,
  SHARK,
  TURTLE
} FishType;

class Fish {
  public:
    Fish( SVGElement* e );

    SVGElement* element; // SVG Object associated with fish
    
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
    virtual void updateFishForce() = 0;

    // Function to make fish interact with mouse click
    virtual void scareFish(double x, double y) = 0;

  protected:

    // Calculate intersections to four walls of fishtank
    std::vector<Vector2D> getIntersections();
    std::vector<Vector2D> corners;

    double force;
    double torque;

    double vel = 0.1; // Linear velocity
    double fearLevel = 0;

    double omega; // Angular velocity

    // List to store distances and headings between fish
    std::vector<double> fishDist;
    std::vector<double> fishHead;

    // Constant scale factors
    const double sx = 0.05;
    const double sy = 0.05;

    const double attractionThresh = 50;
    const double repulsionThresh = 25;
    const double scareThresh = 100;

    double velMax = 0.15;
    double velMin = 0.05;

};

class Minnow: public Fish {
  public:
    Minnow(SVGElement* e): Fish(e){};
    ~Minnow() {};

    virtual void updateFishForce();
    virtual void scareFish(double x, double y);
};

/*class Shark: public Fish {

};

class Turtle: public Fish {

};*/

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