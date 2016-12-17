#include "CMU462.h"
#include "viewer.h"
#include "drawsvg.h"
#include "fish.h"

#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace CMU462;

#define msg(s) cerr << "[DrawSVG] " << s << endl;

int loadAquarium( DrawSVG* drawsvg) {

  SVG* svg = new SVG();

  if( SVGParser::load( drawsvg->aquariumInfo, svg ) < 0) {
    delete svg;
    return -1;
  }
  
  drawsvg->newTab( svg );
  return 0;
}

int main( int argc, char** argv ) {

  // create viewer
  Viewer viewer = Viewer();

  // create drawsvg
  DrawSVG* drawsvg = new DrawSVG();

  // set drawsvg as renderer
  viewer.set_renderer(drawsvg);

  // load tests

  if(argc != 4) {
    msg("Usage: ./aquarium #fish #sharks #turtles "); 
    exit(0);
  }

  int numMinnows = atoi(argv[1]);
  int numTurtles = atoi(argv[2]);
  int numSharks = atoi(argv[3]);

  AquariumInfo ai;

  ai.numMinnows = numMinnows;
  ai.numSharks = numSharks;
  ai.numTurtles = numTurtles;

  drawsvg->aquariumInfo = ai;

  loadAquarium(drawsvg);

  SVG* svg = drawsvg->tabs[drawsvg->current_tab];

  // Create minnow obejcts
  for (size_t i=0; i<numMinnows; ++i) {
    SVG* svg = drawsvg->tabs[drawsvg->current_tab];
    int randomInt = rand() % 4 +1; // int between 1-4

    // initialize fish with one of the 4 color
    if (randomInt == 1) {
      svg->elements[i]->style.fillColor = Color( 0.8,0.1,0.1 );
    }
    else if (randomInt == 2) {
      svg->elements[i]->style.fillColor = Color( 0.541176,0.168627, 0.886275);
    }
    else if (randomInt == 3) {
      svg->elements[i]->style.fillColor = Color( 0.8,0.8,0.1 );
    }
    else {
      svg->elements[i]->style.fillColor = Color( 0.133333, 0.545098, 0.133333);
    }
    Minnow* minnow = new Minnow(svg->elements[i]);
    svg->fish.push_back(minnow);
  }

  // Create turtle obejcts
  for (size_t i=numMinnows; i<numMinnows+numTurtles; ++i) {
    SVG* svg = drawsvg->tabs[drawsvg->current_tab];
    svg->elements[i]->style.fillColor = Color( 0.33333, 0.33333, 0.33333);
    Turtle* turtle = new Turtle(svg->elements[i]);
    svg->fish.push_back(turtle);
  }

  // Create shark objects
  for (size_t i=numMinnows+numTurtles; i<numMinnows+numSharks+numTurtles; ++i) {
    SVG* svg = drawsvg->tabs[drawsvg->current_tab];
    Shark* shark = new Shark(svg->elements[i]);
    svg->elements[i]->style.fillColor = Color( 0.1,0.1,0.1 );
    svg->fish.push_back(shark);
  }

  cout << "PRESS SPACE TO START ANIMATION" << endl;

  // init viewer
  viewer.init();

  // start viewer
  viewer.start();

  return 0;
}
