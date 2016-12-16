# Partical Systems: Animating Fish Behaviors
### Rick Shanor and Pratik Chatrath

For this assignment, we  developed a  2D fishtank simulator to animate schooling/shoaling behaviors of fish based on partical interactions. Our final demo shows an animation consisting of prey (schools of fish) and predators (sharks) that is forward simulated based on simple interactions between the agents in the environment. Prey have position, velocity and heading attributes. Attraction and repulsion forces generate swarm behaviors. Predators have similar attributes. Predators have the ability to eat prey. A motivating image is shown below.    

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

### Work Breakdown

The following concrete steps will be required for successful completion of this project.

 1. Create/find SVGs for fish and sharks and extend HW1 rasterizer as necessary to draw these files in the viewer.  
 2. Create classes for fish and sharks. Pull in relevent animation code from HW4 such that we can forward simulate objects motion based on position and velocity.
 3. Write code calculating the repulsive and attractive forces between agents in the environment. Using a K-D tree structure may be required to search efficiently.
 4. Integrate forces calculated in #3 to update the velocities in #2. At this point, we should have the minimally viable partical simulator in place. 
 5. Extend prey class for reproduction and predator class for feeding to generate interesting, natural behaviors.

#### References

This assignment was completed for Carnegie Mellons Computer Graphics class (15-462), taught by Professors Keenan Crane and Stelian Coros. More info can be found on the course website here: http://15462.courses.cs.cmu.edu/fall2016/home

[1] School of fish simulation video: https://www.youtube.com/watch?v=NKhHp7Wfdmk

[2] Schooling fish behavior: https://www.mrl.ucsb.edu/sites/default/files/mrl_docs/ret_attachments/research/KTaylor.pdf

[3] Image: https://s-media-cache-ak0.pinimg.com/564x/b4/87/a1/b487a19e031b7d220e5638cd5c69c109.jpg

[4] Flocking video: https://www.youtube.com/watch?v=QbUPfMXXQIY&feature=youtu.be

[5] Coordinated motion: http://www.red3d.com/cwr/boids/
