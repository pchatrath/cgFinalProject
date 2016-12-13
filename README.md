# Partical Systems: Animating Fish Behaviors
### Rick Shanor and Pratik Chatrath

For this assignment, we plan to to develop a 2D fishtank simulator to investigate animating natural behaviors based on partical interactions. Our final goal is to build an animation consisting of prey (schools of fish) and predators (sharks) that will forward simulate based on simple interactions between the agents in the environment. Prey will have attributes such as position, velocity, heading. Attraction and repulsion forces will generate swarm behaviors. In addition, prey will have the ability to reproduce. Predators will have similar attributes, however their forces will be generated based on their hunger level. Predators will have the ability to eat prey. A motivating image is shown below.    

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

Finally, press space bar to start and stop the animation.

### Work Breakdown

The following concrete steps will be required for successful completion of this project.

 1. Create/find SVGs for fish and sharks and extend HW1 rasterizer as necessary to draw these files in the viewer.  
 2. Create classes for fish and sharks. Pull in relevent animation code from HW4 such that we can forward simulate objects motion based on position and velocity.
 3. Write code calculating the repulsive and attractive forces between agents in the environment. Using a K-D tree structure may be required to search efficiently.
 4. Integrate forces calculated in #3 to update the velocities in #2. At this point, we should have the minimally viable partical simulator in place. 
 5. Extend prey class for reproduction and predator class for feeding to generate interesting, natural behaviors.

#### References

[1] School of fish simulation video: https://www.youtube.com/watch?v=NKhHp7Wfdmk

[2] Schooling fish behavior: https://www.mrl.ucsb.edu/sites/default/files/mrl_docs/ret_attachments/research/KTaylor.pdf

[3] Image: https://s-media-cache-ak0.pinimg.com/564x/b4/87/a1/b487a19e031b7d220e5638cd5c69c109.jpg

[4] Flocking video: https://www.youtube.com/watch?v=QbUPfMXXQIY&feature=youtu.be

[5] Coordinated motion: http://www.red3d.com/cwr/boids/
