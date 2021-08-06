# CS3258 Intro to Graphics Project
Author: Joseph Chang

Purpose: A command line interface for manipulating TIFF images and rendering basic shapes to the screen

# Compilation Instructions
1. `cd` into the root directory
2. run ```make```
3. run ```./main```

# Manual
Note: Brackets specify parameters and should not be included when actually performing the command.

Note: BE sure to only have 1 delimiter (comma, space) between each argument. Consecutive delimiters with no arguments in between will be interpreted as a 0 argument.

## Dummy Commands
The following commands take floating point arguments and simply echo what they're supposed to do:

```
move [x,y,z]
```
move the pen to (x, y, z)

```
draw [x,y,z]
```
draw with the pen to (x, y, z)

```
color [v,h,s]
```
set the color to v, h, s

## TIFF Reader/Writer
```
tiffread [f]
```
read a tiff file with filename `f`

```
tiffstat [f]
```
print the parameters of tiff image with filename `f`

```
tiffwrite [f,x0,y0,xc,yc]
```
write a tiff file to filename `f` from the box (x0, y0, xc, yc)

Note: `tiffread` must be executed first before `tiffwrite` can be used.

## Resizer
Note: Both of these commands require `tiffread` to be executed first before they can be used.


Both of these commands take non-negative floating point arguments:

```
resize [xscale,yscale]
```
resize by xscale in x and yscale in y

```
zoom [s]
```
scales the image in both directions by the amount s

prints out the dimensions of the new image for easy writing

## 3D Transformations
All of the following commands take floating point arguments:
```
push
```
push the current transformation matrix on the stack

```
pop
```
pop the top of the matrix stack and store it into the current transformation matrix

```
translate [x,y,z]
```
translate the origin by the vector <x, y, z>

```
scale [sx,sy,sz]
``` 
scale in x by s<sub>x</sub>, scale in y by s<sub>y</sub>, and scale in z by s<sub>z</sub>

```
rotate [theta,ax,ay,az]
```
rotate theta degrees about the axis (a<sub>x</sub>, a<sub>y</sub>, a<sub>z</sub>)

```
ortho [l,r,b,t,n,f]
```
implement an orthographic projection

```
perspective [theta,a,n,f]
```
implement a perspective transform

```
lookat [fx,fy,fz,ax,ay,az,ux,uy,uz]
```
change the point of view

```
vertex [x,y,z]
```
draw a line between two points

Note: be sure to call `reset` before drawing to render on a black screen.

```
reset
```
clears the screen and the matrix stack


# Extra Credit
  For assignment #5, the rendering of the script for this project, which is called project5test.cli in the script folder, took 717 milliseconds to render.
  The tiff image corresponding to the script is reflect.tiff, which is in the images folder.
