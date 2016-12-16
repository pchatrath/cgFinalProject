# Partical Systems: Animating Fish Behaviors
### Rick Shanor and Pratik Chatrath

For this assignment, we  developed a  2D fishtank simulator to animate schooling/shoaling behaviors of fish based on partical interactions. Our final demo shows an animation consisting of prey (schools of fish) and predators (sharks) that is forward simulated based on simple interactions between the agents in the environment. Prey have position, velocity and heading attributes. Attraction and repulsion forces generate swarm behaviors. Predators have similar attributes. Predators have the ability to eat prey. Aditionally mouse click on a particular position scares a fish. A motivating image is shown below.    

![picture alt](https://s-media-cache-ak0.pinimg.com/564x/b4/87/a1/b487a19e031b7d220e5638cd5c69c109.jpg)

### Usage Instructions

First build the project
```
mkdir build && cd build && cmake .. && make
```

Next, run the program from the command line
```
./aquarium #fish #turtles #sharks
```

Finally, press space bar to start and stop the animation. Click with the mouse to scare fish.

### Features Implemented & Implementation Detail

#### The following were the desired steps to be achieved by this project

 1. Create/find SVGs for fish and sharks and extend HW1 rasterizer as necessary to draw these files in the viewer.  
 2. Create classes for fish and sharks. Pull in relevent animation code from HW4 such that we can forward simulate objects motion based on position and velocity.
 3. Write code calculating the repulsive and attractive forces between agents in the environment. Using a K-D tree structure may be required to search efficiently.
 4. Integrate forces calculated in #3 to update the velocities in #2. At this point, we should have the minimally viable partical simulator in place. 
 5. Extend prey class for reproduction and predator class for feeding to generate interesting, natural behaviors.

#### The following steps were implemented  at the end of the project to get the above mentioned desired behavior

 1. SVG files were obtained from web for fish and sharks. These files are defined in terms of polygon and are supported by our SVGParser
 2. Created Class for fish. Each fish has its position, velocity and heading. Extended Fish class to Minnow (kind of fish) and sharks classes. Updated DrawSVG time_event to update posiiton, velocity, heading at each instance of time.
 3. To calcualte the updated position, velocity, heading of each fish at a time instance following rules were implemented
    a. Fish of same type attract each other.
    b. If fishes of same type come very close to each other they experinece repulsion forces
    c. Minnow try to avoid shark 
4. Using rules mentioned in step 3 a functioning school of fish behavior was obtained
5. Sharks demonstrate predator behavior by consuming Minnow fishes which are very close to it

#### Extra features implemented:
1. Mouse clicks exhibit a fish scare behavior
2. Added support for displaying turtle

### Difficulties in implementation
1. We faced issue confinig fish movement to a bounded fish tank. We got rid of this issue by creating unbounded fish tank. The complete screen is now used as fish tank and fish can enter/exit from any direction
2. Secondly for implementing behavior of Minnow fishes avoiding sharks, minnow fishes did not avoid sharks. The minnow sidhes  avoided some part behing the shark in the svg. We tried to re-center the shark svg so that the position of shark corresponds to center of shark in the svg. However we were not able to re-center the svg. We worked around this issue by cretign a bounding box around the shark and making the fish to avoid the bounding box.
3. The most difficut part was to implement natural/realistic fish behavior. We worked on fine tuning the attraction, repulsion, and other constants related to updating position, velocity, heading to get realistic behavior. 

### Learning Outcomes of this assignment

### References

This assignment was completed for Carnegie Mellons Computer Graphics class (15-462), taught by Professors Keenan Crane and Stelian Coros. More info can be found on the course website here: http://15462.courses.cs.cmu.edu/fall2016/home

[1] School of fish simulation video: https://www.youtube.com/watch?v=NKhHp7Wfdmk

[2] Schooling fish behavior: https://www.mrl.ucsb.edu/sites/default/files/mrl_docs/ret_attachments/research/KTaylor.pdf

[3] Image: https://s-media-cache-ak0.pinimg.com/564x/b4/87/a1/b487a19e031b7d220e5638cd5c69c109.jpg

[4] Flocking video: https://www.youtube.com/watch?v=QbUPfMXXQIY&feature=youtu.be

[5] Coordinated motion: http://www.red3d.com/cwr/boids/
