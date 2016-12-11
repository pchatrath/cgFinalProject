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

/*int loadDirectory( DrawSVG* drawsvg, const char* path ) {

  DIR *dir = opendir (path);
  if(dir) {
    
    struct dirent *ent; size_t n = 0;
    
    // load files
    string pathname = path; 
    if (pathname[pathname.back()] != '/') pathname.push_back('/');
    while (((ent = readdir (dir)) != NULL) && (n < 9)) {

      string filename = ent->d_name;
      string filesufx = filename.substr(filename.find_last_of(".") + 1);
      if (filesufx == "svg" ) {
        cerr << "[DrawSVG] Loading " << filename << "... "; 
        if (loadFile(drawsvg, (pathname + filename).c_str()) < 0) {
          cerr << "Failed (Invalid SVG file)" << endl;
        } else {
          cerr << "Succeeded" << endl;
          n++;
        }
      }
    }

    closedir (dir);

    if (n) {
      msg("Successfully Loaded " << n << " files from " << path);
      return 0;
    }

    msg("No valid svg files found in " << path);
    return -1;
  } 

  msg("Could not open directory" << path);
  return -1;
}*/


int main( int argc, char** argv ) {

  // create viewer
  Viewer viewer = Viewer();

  // create drawsvg
  DrawSVG* drawsvg = new DrawSVG();

  // set drawsvg as renderer
  viewer.set_renderer(drawsvg);

  // load tests
/*  if( argc == 2 ) {
    if (loadPath(drawsvg, argv[1]) < 0) exit(0);
  } else {
    msg("Usage: drawsvg <path to test file or directory>"); exit(0);
  }*/

  if(argc != 4) {
    msg("Usage: ./aquarium #fish #sharks #turtles "); 
    exit(0);
  }

  char minnowPath[] = "../svg/minnow.svg";
  char turtlePath[] = "../svg/turtle.svg";
  char sharkPath[] = "../svg/shark.svg";

  int numMinnows = atoi(argv[1]);
  int numSharks = atoi(argv[2]);
  int numTurtles = atoi(argv[3]);

  AquariumInfo ai;

  ai.numMinnows = numMinnows;
  ai.numSharks = numSharks;
  ai.numTurtles = numTurtles;

  drawsvg->aquariumInfo = ai;

  loadAquarium(drawsvg);

  SVG* svg = drawsvg->tabs[drawsvg->current_tab];

  for (size_t i=0; i<numMinnows; ++i) {
    SVG* svg = drawsvg->tabs[drawsvg->current_tab];
    Minnow* minnow = new Minnow(svg->elements[i]);
    svg->fish.push_back(minnow);
  }
  
  for (size_t i=numMinnows; i<numMinnows+numSharks; ++i) {
    SVG* svg = drawsvg->tabs[drawsvg->current_tab];
    Shark* shark = new Shark(svg->elements[i]);
    svg->fish.push_back(shark);
  }

  for (size_t i=numMinnows+numSharks; i<numMinnows+numSharks+numTurtles; ++i) {
    SVG* svg = drawsvg->tabs[drawsvg->current_tab];
    Turtle* turtle = new Turtle(svg->elements[i]);
    svg->fish.push_back(turtle);
  }

  cout << "PRESS SPACE TO START ANIMATION" << endl;

  // init viewer
  viewer.init();

  // start viewer
  viewer.start();

  return 0;
}
