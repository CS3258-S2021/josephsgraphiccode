#ifndef GLOBAL_H
#define GLOBAL_H

extern "C" {
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
}

#include "CLI.h"

/*	Create checkerboard image	*/
#define	checkImageWidth 1024
#define	checkImageHeight 1024
extern GLubyte checkImage[checkImageHeight][checkImageWidth][3];

extern void makeCheckImage();
extern void makeBlackImage();

extern void draw_line(float, float, float, float, CLI&);
extern int near_far_clip(float, float, float *, float *, float *, float *, float *, float *);

#endif // GLOBAL_H
