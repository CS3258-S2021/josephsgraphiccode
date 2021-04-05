#ifdef __cplusplus
extern "C" {
#endif

/*-- NOTE: You will have to manipulate this code to correspond with the
            structures and stack implementation you chose to use. 
	    Don't forget to put declarations in your .h files. 
           
            See the supplemental handout for information on integrating C and
            C++ code.

--*/


/* The following (not including LookAt and Vertex3f) could either be 
   implemented into your file or they can stand as a separate file called 
   lines.c   It will be used for drawing pixels to the screen and 
   for clipping. */

#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>


/* the clipping window */
static float xmin = 0.0;
static float xmax = 1.0;
static float ymin = 0.0;
static float ymax = 1.0;

static void set_clip_window(float, float, float, float);
static int clip_line(float *, float *, float *, float *);


/* These go in your .h file or in lines.h */
/*

Line drawing header.

*/


void draw_line(float, float, float, float);
int near_far_clip(float, float, float *, float *, float *, float *,
                  float *, float *);


/******************************************************************************
Draw a white line.

Entry:
  x0,y0 - first endpoint of line
  x1,y1 - second line endpoint
******************************************************************************/

void draw_line(float x0, float y0, float x1, float y1)
{
  GLint viewport[4];
  int i;
  float x,y;
  float dx,dy;
  float xinc,yinc;
  float length=0.;
  int result;
  int width,height;

  /* set the clipping window */
  
  glGetIntegerv(GL_VIEWPORT, viewport);
  width = abs(viewport[2]-viewport[0]);
  height = abs(viewport[3]-viewport[1]);

  set_clip_window (0.0, 0.0, width - 0.51, height - 0.51);

  /* clip the line in 2D */
  result = clip_line (&x0, &y0, &x1, &y1);

  /* return if line is entirely outside the clip window */
  if (result == 0)
    return;

  /* incremental line drawing */

  dx = x1 - x0;
  dy = y1 - y0;

  /* determine whether horizontal or vertical change is larger */

  if (fabs(dx) > fabs(dy))
    length = fabs(dx);
  else
    length = fabs(dy);

  /* special case to avoid dividing by zero */

  if (length == 0) {
    glColor3f( 1.0, 1.0, 1.0 );
    glBegin(GL_POINTS);
    glVertex2i((int) floor(x0+0.5), (int) floor(y0+0.5));
    glEnd();
   
    return;
  }

  xinc = dx / length;
  yinc = dy / length;

  x = x0;
  y = y0;

  /* write "length" number of pixels along the line */

  for (i = 0; i <= length; i++) {
    glColor3f( 1.0, 1.0, 1.0 );
    glBegin(GL_POINTS);
    glVertex2i((int) floor(x+0.5), (int) floor(y+0.5));
    glEnd();
    

    x += xinc;
    y += yinc;
  }
  glFlush();
}


/******************************************************************************
Specify a clipping window.

Entry:
  x0,y0 - lower left boundaries of clipping window
  x1,y1 - upper right boundaries
******************************************************************************/

static void set_clip_window(float x0, float y0, float x1, float y1)
{
  xmin = x0;
  ymin = y0;

  xmax = x1;
  ymax = y1;
}


/******************************************************************************
Given a point P outside the window and the rise and run of a line, return
the intersection of line with window that is nearest P.

Entry:
  dx,dy - run and rise of line
  x,y   - the given point P

Exit:
  ix,iy - intersection point
  return 1 if there was a valid intersection, 0 if not
******************************************************************************/

static int clip_helper(
  float dx,
  float dy,
  float x,
  float y,
  float *ix,
  float *iy
)
{
  /* if line not vertical, check against left and right edges of window */

  if (dx != 0) {

    /* check against left edge */
    *iy = dy / dx * (xmin - x) + y;
    if (xmin >= x && *iy >= ymin && *iy <= ymax) {
      *ix = xmin;
      return (1);
    }

    /* check against right edge */
    *iy = dy / dx * (xmax - x) + y;
    if (xmax <= x && *iy >= ymin && *iy <= ymax) {
      *ix = xmax;
      return (1);
    }
  }

  /* if line not horizontal, check against top and bottom edges of window */

  if (dy != 0) {

    /* check against bottom edge */
    *ix = dx / dy * (ymin - y) + x;
    if (ymin >= y && *ix >= xmin && *ix <= xmax) {
      *iy = ymin;
      return (1);
    }

    /* check against top edge */
    *ix = dx / dy * (ymax - y) + x;
    if (ymax <= y && *ix >= xmin && *ix <= xmax) {
      *iy = ymax;
      return (1);
    }
  }

  /* if we get here, we found no intersection */
  return (0);
}


/******************************************************************************
Clip a line segment to a pre-specified window.

Entry:
  x0,y0 - first line segment endpoint
  x1,y1 - second endpoint

Exit:
  x0,y0,x1,y1 - clipped endpoint positions
  returns 1 if segment is at least partially in window,
  returns 0 if segment is entirely outside window
******************************************************************************/

static int clip_line(float *x0, float *y0, float *x1, float *y1)
{
  int count;
  float dx,dy;
  float xx0 = *x0;
  float yy0 = *y0;
  float xx1 = *x1;
  float yy1 = *y1;

  int code04 = (xx0 < xmin) ? 1 : 0;
  int code03 = (xx0 > xmax) ? 1 : 0;
  int code02 = (yy0 < ymin) ? 1 : 0;
  int code01 = (yy0 > ymax) ? 1 : 0;

  int code14 = (xx1 < xmin) ? 1 : 0;
  int code13 = (xx1 > xmax) ? 1 : 0;
  int code12 = (yy1 < ymin) ? 1 : 0;
  int code11 = (yy1 > ymax) ? 1 : 0;

  int sum0 = code01 + code02 + code03 + code04;
  int sum1 = code11 + code12 + code13 + code14;

  /* completely inside window? */
  if (sum0 == 0 && sum1 == 0)
    return (1);

  /* check for trivial invisibility (both endpoints on wrong side of */
  /* a single side of the window) */

  if ((code01 && code11) || (code02 && code12) || (code03 && code13) ||
      (code04 && code14)) {
    return (0);
  }

  /* compute run and rise */
  dx = xx1 - xx0;
  dy = yy1 - yy0;

  /* case: only x0,y0 is inside window */
  if (sum0 == 0) {
    //int dummy = 
	clip_helper (dx, dy, xx1, yy1, &xx1, &yy1);
    *x0 = xx0;
    *y0 = yy0;
    *x1 = xx1;
    *y1 = yy1;
    return (1);
  }

  /* case: only x1,y1 is inside window */
  if (sum1 == 0) {
   // int dummy = 
	clip_helper (dx, dy, xx0, yy0, &xx0, &yy0);
    *x0 = xx0;
    *y0 = yy0;
    *x1 = xx1;
    *y1 = yy1;
    return (1);
  }

  /* neither endpoint is inside the window */

  count = 0;
  count += clip_helper (dx, dy, xx0, yy0, &xx0, &yy0);
  count += clip_helper (dx, dy, xx1, yy1, &xx1, &yy1);

  *x0 = xx0;
  *y0 = yy0;
  *x1 = xx1;
  *y1 = yy1;

  if (count)
    return (1);
  else
    return (0);
}


/******************************************************************************
Clip a line segment to front and back clipping planes.  These clip planes
are along the z-axis.  If your objects are on the negative z portion of
the axis, be sure to specify negative values for "near" and "far".

Entry:
  near,far - clip planes along z-axis
  x0,y0,z0 - first line segment endpoint
  x1,y1,z1 - second endpoint

Exit:
  x0,y0,z0,x1,y1,z1 - clipped endpoint positions
  returns 1 if segment is at least partially in window,
  returns 0 if segment is entirely outside window
******************************************************************************/

int near_far_clip(
  float near,
  float far,
  float *x0,
  float *y0,
  float *z0,
  float *x1,
  float *y1,
  float *z1
)
{
  float temp;
  float fract;
  float xx0 = *x0;
  float yy0 = *y0;
  float zz0 = *z0;
  float xx1 = *x1;
  float yy1 = *y1;
  float zz1 = *z1;
  int code00,code01,code10,code11;

  /* make sure near < far */

  if (near > far) {
    temp = far;
    far = near;
    near = temp;
  }

  /* figure out which endpoints are outside the clipping volume */

  code00 = (zz0 < near) ? 1 : 0;
  code01 = (zz0 > far)  ? 1 : 0;
  code10 = (zz1 < near) ? 1 : 0;
  code11 = (zz1 > far)  ? 1 : 0;

  /* return without clipping if all endpoints are inside clip volume */

  if (code00 + code01 + code10 + code11 == 0)
    return (1);  /* signals inside volume */

  /* if both endpoints are entirely out of clip volume, exit and signal this */

  if ((code00 && code10) || (code01 && code11))
    return (0);  /* signals outside volume */

  /* clip to near plane if necessary */

  if (code00) {
    fract = (near - zz0) / (zz1 - zz0);
    xx0 = xx0 + fract * (xx1 - xx0);
    yy0 = yy0 + fract * (yy1 - yy0);
    zz0 = near;
  }
  else if (code10) {
    fract = (near - zz1) / (zz0 - zz1);
    xx1 = xx1 + fract * (xx0 - xx1);
    yy1 = yy1 + fract * (yy0 - yy1);
    zz1 = near;
  }

  /* clip to far plane if necessary */

  if (code01) {
    fract = (far - zz0) / (zz1 - zz0);
    xx0 = xx0 + fract * (xx1 - xx0);
    yy0 = yy0 + fract * (yy1 - yy0);
    zz0 = far;
  }
  else if (code11) {
    fract = (far - zz1) / (zz0 - zz1);
    xx1 = xx1 + fract * (xx0 - xx1);
    yy1 = yy1 + fract * (yy0 - yy1);
    zz1 = far;
  }

  /* copy the clipped endpoints */

  *x0 = xx0;
  *y0 = yy0;
  *z0 = zz0;
  *x1 = xx1;
  *y1 = yy1;
  *z1 = zz1;

  /* signal that we're inside the clip volume */
  return (1);
}

/*=================== End of lines.c =====================================*/


/*  Implement the following routines into your file */


#define ROW 4
#define COL 4


/* Definitions of the types of structures and variables used in the following 
   code.  Use these or implement your own. */

typedef struct                  /* structure definitions */
{
   float  mat[ROW][COL];
}  matrix_unit; 

typedef struct
{
   float i;
   float j;
   float k;
} Vector;

typedef struct
{
   float x;
   float y;
   float z;
   float r;
   float g;
   float b;
} Vertex_unit;

typedef struct
{
  float mat41[ROW];
} matrix41;

static float Near, Far;

float w;

int perspflag=0;

static matrix_unit I = {
   { {1., 0., 0., 0.},
     {0., 1., 0., 0.},
     {0., 0., 1., 0.},
     {0., 0., 0., 1.}  },
};  

matrix_unit *stack[50];    /* array of pointers to act as a stack */

int top = 0;                   /* points to top of the stack */

int width, height;         /* height and width of frame buffer */

static matrix_unit orth;       /* global ortho and perspective matrices */
                               /* to be used in Vertex3f */    
static matrix_unit perspect;


/* These go in your .h file */
void gtLookAt( float fx, float fy, float fz, float atx, float aty, 
	       float atz, float upx, float upy, float upz);
void gtVertex3f(float x, float y, float z);

/*========================================================================*/
/* Finds a cross product of two vectors                                   */
/*========================================================================*/
void Cross(Vector *x, Vector *y, Vector *z) {
    
    z->i = x->j * y->k - x->k * y->j;
    z->j = x->k * y->i - x->i * y->k;
    z->k = x->i * y->j - x->j * y->i;
}


/*========================================================================*/
/* Finds a unit vector in the direction of a given vector                 */
/*========================================================================*/
void Unitvec(float x, float y, float z, Vector *vec){
    float vec_length;
    
    vec_length = sqrt(x*x + y*y + z*z);
    vec->i = x/vec_length;
    vec->j = y/vec_length;
    vec->k = z/vec_length;
}

/*========================================================================*/
/* Multiplies a 4x4 matrix by a 4x1 vector                                */
/* Returns 0 upon success and 1 if pointers are not pointing to matrices  */
/*========================================================================*/
int Mult_end(matrix_unit *M, matrix41 *V, matrix41 *result) {
    int i, j;

    if (M == NULL || V == NULL || result == NULL) {
        printf("error in matrix multiplication \n");
        return 1;
    }
    for (i=0; i<4; i++) {
            (result->mat41)[i] = 0;
        for (j=0; j<4; j++) 
                (result->mat41)[i] += (M->mat)[i][j]*(V->mat41)[j];
    }

    return 0;
}

/*========================================================================*/
/* Multiplies two 4x4 matrices                                            */
/* Returns 0 upon success and 1 if pointers are not pointing to matrices  */
/*========================================================================*/
int Mult_mat(matrix_unit *left, matrix_unit *right, matrix_unit *result) {
    int i, j, k;

    if (left == NULL || right == NULL || result == NULL) {
        printf("error in matrix multiplication -- matrix not allocated\n");
        return 1;
    }
    for (i=0; i<4; i++) {
        for (j=0; j<4; j++) {
            (result->mat)[i][j] = 0;
            for (k=0; k<4; k++)
                (result->mat)[i][j] += (left->mat)[i][k]*(right->mat)[k][j];
        }
    }
    return 0;
}

int Copy_mat(matrix_unit *from, matrix_unit *to) {
    int i, j;
    if (from != NULL && to != NULL) {
        for (i=0; i<ROW; i++) 
            for (j=0; j<ROW; j++) 
                (to->mat)[i][j] = (from->mat)[i][j];
        return 0;
    }
    else {
        printf(" Cannot copy matrix\n");
        return 1;
    }
}


/*========================================================================*/
/*gtLookAt allows a change of the point of view from which the scene is
  is looked at.  (like a virtual camera)*/
/*========================================================================*/

void gtLookAt( float fx, float fy, float fz, float atx, float aty, 
	       float atz, float upx, float upy, float upz)
{
   float dx, dy, dz ;
   Vector slnv, rx, ry, rz, up;
   matrix_unit ltrans, tmpsln, *t, rfin, lookat;
   //Vertex_unit v, sv;
   //int i, j;

   /*   translation */
   ltrans = I;
   t = &ltrans;
   t->mat[0][3] = -fx;
   t->mat[1][3] = -fy;
   t->mat[2][3] = -fz; 

   up.i = upx; up.j = upy; up.k = upz;
   Unitvec(up.i, up.j, up.k, &slnv);
   up = slnv;

   /*  make P1P2  (rz) vector */
   dx = atx - fx;   dy = aty - fy;  dz = atz - fz;
   Unitvec( dx, dy, dz, &rz);  

   rfin = I;
      /* make rx */
   Cross(&rz, &up, &rx);  
   Unitvec( rx.i, rx.j, rx.k, &slnv);
   rx = slnv;

      /* make ry */
   Cross(&rx, &rz, &ry);  

   rfin.mat[0][0] = rx.i; rfin.mat[0][1] = rx.j; rfin.mat[0][2] = rx.k;
   rfin.mat[1][0] = ry.i; rfin.mat[1][1] = ry.j; rfin.mat[1][2] = ry.k;
   rfin.mat[2][0] = -rz.i; rfin.mat[2][1] = -rz.j; rfin.mat[2][2] = -rz.k;

   Mult_mat(&rfin, &ltrans, &lookat);
  
   /* Multiply the lookat matrix by the matrix currently on the top 
      of the stack.  This then becomes the new top of the stack. */

   /*  This will depend on how you implemented your stack */
  Mult_mat(stack[top], &lookat, &tmpsln);  
  Copy_mat(&tmpsln, stack[top]);
   
}
/*------------------------------End of gtLookAt -------------------------*/

/*=====================================================================*/
/*gtVertex3f specifies a 3D vertex that is a line endpoint.
  White lines are drawn between successive odd/even pairs of 
  these vertices.*/
/*=====================================================================*/

void gtVertex3f(float x, float y, float z)
 
{
  matrix41 printvec, tmp, vertex1, vertex2, pvert1, pvert2, permat1, permat2;
  float x0, y0, z0, x1, y1, z1;
  static int printflag=0;
  static matrix41 savemat;
  //float xpixel1, ypixel1, xpixel2, ypixel2;

  printvec.mat41[0]=x;    /* set up 4x1 matrix of the vertex */
  printvec.mat41[1]=y;
  printvec.mat41[2]=z;
  printvec.mat41[3]=1;

/*Mult_end is a function to multiply matrix a * matrix b and put the
  solution in sln.  This function multiplies [4][4]*[4][1].  This will
  vary depending on your implementation. */

  Mult_end(stack[top],&printvec, &tmp);

  printflag++;       /* increase counter */
  
  if(printflag==1) { /* if the first vertex, save the points */
    savemat.mat41[0]=tmp.mat41[0];
    savemat.mat41[1]=tmp.mat41[1];
    savemat.mat41[2]=tmp.mat41[2];
    savemat.mat41[3]=1;
  }
  if(printflag==2) { /* if the second vertex, test clipping */

    x1=tmp.mat41[0];
    y1=tmp.mat41[1];
    z1=tmp.mat41[2];

    x0=savemat.mat41[0];
    y0=savemat.mat41[1];
    z0=savemat.mat41[2];
  
    /* if clipping occurs and points are within view volume, draw line */
    /* from v1 to v2 */
    if(near_far_clip(Near, Far, &x0, &y0, &z0, &x1, &y1, &z1)==1) {
	
	pvert1.mat41[0]=x0;
	pvert1.mat41[1]=y0; 
	pvert1.mat41[2]=z0;
	pvert1.mat41[3]=1;
	pvert2.mat41[0]=x1;
	pvert2.mat41[1]=y1;
	pvert2.mat41[2]=z1;
	pvert2.mat41[3]=1;

	if(perspflag==0) {  /* if not a perpective projection, use ortho */

	  Mult_end(&orth, &pvert1, &vertex1); /* calculate 2d coordinates */
	  Mult_end(&orth, &pvert2, &vertex2);

	  draw_line(vertex1.mat41[0], vertex1.mat41[1], 
		    vertex2.mat41[0], vertex2.mat41[1]);
	  
	  printflag=0;
	}
	else {   /* if a perspective projection, use persp */
	  
	  permat1.mat41[0]=x0/abs(z0);   /* divide by abs(z) to account for */
	  permat1.mat41[1]=y0/abs(z0);  /* z=1 assumption in gtPerspective */
	  permat1.mat41[2]=1.0;
	  permat1.mat41[3]=(float)(1/abs(z0));
	  permat2.mat41[0]=x1/abs(z1);
	  permat2.mat41[1]=y1/abs(z1);
	  permat2.mat41[2]=1.0;
	  permat2.mat41[3]=(float)(1/abs(z1));
	  
	  Mult_end(&perspect, &permat1, &vertex1);
	  Mult_end(&perspect, &permat2, &vertex2);
	  
	  //BOBBY
	  printf(" Vertex 1: %f %f\n Vertex 2: %f %f\n",vertex1.mat41[0],vertex1.mat41[1],vertex2.mat41[0],vertex2.mat41[1]);

	  draw_line(vertex1.mat41[0], 
		    vertex1.mat41[1], 
		    vertex2.mat41[0], 
		    vertex2.mat41[1]);
	   

	  printflag=0; /* set counter=0 to look for another set of vertices */
	}
    }

    else
      printflag=0;  /* if vertices beyond clipping plane, reset counter */

  }
}
  
/*--------------------End of gtVertex3f----------------------------------*/


#ifdef __cplusplus
}
#endif


