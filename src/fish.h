#ifndef CMU462_FISH_H
#define CMU462_FISH_H

#include "vector2D.h"
#include "svg.h"

namespace CMU462 {

// Required to avoid circular dependancies
class SVGElement;

class Fish {
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


} // namespace CMU462

#endif // CMU462_FISH_H