# Part 1

![alt-text](https://github.com/sbme-tutorials/cg-task3-_team-16/blob/main/Part1/samples/shot.png)

## How To Use
**Camera Movements**
-   Up arrow --->Move Up
-   Down arrow ---> Move Down
-   Left arrow ---> Move Left
-   Right arrow ---> Move Right
- ( i )   --->  zoom in
- ( o )  --->  zoom out
- ( r )  ---> Reset

**Body Movements**
 - Shoulders ---> S
 - Left elbow ---> c
 - Right elbow ---> d
 - Left Leg ---> num 1 & num 2
 - Right Leg ---> num 3 & num 4
 - Left knee --->  num 5 & num 6
 - Right Knee ---> num 7 & num 8

**Menu**

Use the menu binned to the right mouse button to 

 - Change the floor
 - Change the animation
 - Reset
 - Exit

##  Floor

- Floor was drawn and placed under the Robot
- Texture map was put on the floor 
- We used 3 different textures that go on with our scene 
-  Use the menu binned to the right mouse button to change the floor texture

![alt-text](https://github.com/sbme-tutorials/cg-task3-_team-16/blob/main/Part1/samples/floor.gif)

## Animations

- We used two objects (Soccer ball and Football-Goal)
- Poses were taken by writing a function to print the angles we want to know there values at certain position and linking this function to the menu so we can call it anytime we want to know the angles 

![alt-text](https://github.com/sbme-tutorials/cg-task3-_team-16/blob/main/Part1/samples/angles.png)

![alt-text](https://github.com/sbme-tutorials/cg-task3-_team-16/blob/main/Part1/samples/angles%20results.png)




**We have two animations here:**

1- Shooting: the robot shoots the ball into the Goal (interacting with objects)

2- Celebrating: the robot celebrates scoring (No interaction with objects)

Use the menu binned to the right mouse button to change the Animation



**sample results**

![alt-text](https://github.com/sbme-tutorials/cg-task3-_team-16/blob/main/Part1/samples/shooting.gif)

![alt-text](https://github.com/sbme-tutorials/cg-task3-_team-16/blob/main/Part1/samples/celebrating.gif)




## Issues 

 1. errors in glm.cpp that took a lot time before it was solved.
 2.  error in glut. h that made the program stop running and the output screen disappeared and after many trials the problem was solved.
 3. mtl and obj of objects ( the soccer ball ) when downloaded from the website, it wasn't like we saw in the section, and I'm not talking about the mtl and the colours, the .obj itself was different. It appeared like it was the negative of the object, we tried to change the colours the materials, changing in the mtl file but we couldn't change it. finally we took the .obj file for the ball from the the tutorial. For the other object (football-goal), we faced the same problem and we think that there's a problem with texture that is with the .obj file, we included all the images the files that were in the .zip file but still have no clue what is the problem.
