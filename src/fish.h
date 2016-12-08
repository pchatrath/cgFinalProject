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

  private:
    Vector2D position; // Fish position
    double heading; // Fish orientation

    double vel; // Linear velocity
    double omega; // Angular velocity

    // Constant scale factors
    const double sx = 0.09;
    const double sy = 0.045;

    void placeFish();

};


} // namespace CMU462

#endif // CMU462_FISH_H