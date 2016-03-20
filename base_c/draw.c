#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"


/*======== void add_circle() ==========
  Inputs:   struct matrix * points
            double cx
	    double cy
	    double y
	    double step  
  Returns: 


  03/16/12 19:53:52
  jdyrlandweaver
  ====================*/
void add_circle( struct matrix * points, 
		 double cx, double cy, 
		 double r, double step ) {
  double t = step;
  double x1,y1;
  int i = 0;
  double x0 = r*cos(0)+cx;
  double y0 = r*sin(0)+cy;
  while (t<1.001){
    x1 = r*cos(t*2*M_PI)+cx;
    y1 = r*sin(t*2*M_PI)+cy;
    if (points->lastcol == points->cols){
      grow_matrix( points, points->lastcol+1 );
      //points->lastcol += 1;
    }
    points->m[0][points->lastcol] = x0;
    points->m[1][points->lastcol] = y0;
    points->m[2][points->lastcol] = 0;
    points->m[3][points->lastcol] = 1;
    i++;
    points->lastcol++;

    if (points->lastcol == points->cols){
      grow_matrix( points, points->lastcol+1 );
      //points->lastcol += 1;
    }
    points->m[0][points->lastcol] = x1;
    points->m[1][points->lastcol] = y1;
    points->m[2][points->lastcol] = 0;
    points->m[3][points->lastcol] = 1;
    i++;
    points->lastcol++;
    //printf("%d\n", points->lastcol);
    
    x0=x1;
    y0=y1;
    t+=step;
  }
}

/*======== void add_curve() ==========
Inputs:   struct matrix *points
         double x0
         double y0
         double x1
         double y1
         double x2
         double y2
         double x3
         double y3
         double step
         int type  
Returns: 

Adds the curve bounded by the 4 points passsed as parameters
of type specified in type (see matrix.h for curve type constants)
to the matrix points

03/16/12 15:24:25
jdyrlandweaver
====================*/
void add_curve( struct matrix *points, 
		double x0, double y0, 
		double x1, double y1, 
		double x2, double y2, 
		double x3, double y3, 
		double step, int type ) {
  struct matrix* Xs;
  struct matrix* Ys;
  
  if (HERMITE_MODE){
    Xs = generate_curve_coefs(x0, x1, x2, x3, type);
    Ys = generate_curve_coefs(y0, y1, y2, y3, type);
  }  
  else if (BEZIER_MODE){
    Xs = generate_curve_coefs(x0, x2, x1-x0, x2-x3, type);
    Ys = generate_curve_coefs(y0, y2, y1-y0, y2-y3, type);
  }

  //print_matrix( Ys );
  //print_matrix( Xs );
  
  //int i=0;
  int X1;
  int Y1;
  int X0;
  int Y0;
  double t = 0;

  //x = t * ( t * (a*t + b) + c) + d;
  X0 = x0;
  
  //y = t * ( t * (a*t + b) + c) + d;
  Y0 = y0;
  
  
  while (t<1.001){

    printf("hi %f\n",t);
    
    //x = t * ( t * (a*t + b) + c) + d;
    //y = t * ( t * (a*t + b) + c) + d;
    
    X1 = t * ( t * ( Xs->m[0][0] *t + Xs->m[1][0]) + Xs->m[2][0]) + Xs->m[3][0];
    Y1 = t * ( t * ( Ys->m[0][0] *t + Ys->m[1][0]) + Ys->m[2][0]) + Ys->m[3][0];
    
    add_edge(points,X0,Y0,0,X1,Y1,0);
    
    /*
    if (points->lastcol == points->cols){
      grow_matrix( points, points->lastcol+1 );
    }
    points->m[0][i] = X1;
    points->m[1][i] = Y1;
    points->m[2][i] = 0;
    points->m[3][i] = 1;
    */
    //points->lastcol++;
    
    X0 = X1;
    Y0 = Y1;
    
    t+= step;

  }
}

/*======== void add_point() ==========
Inputs:   struct matrix * points
         int x
         int y
         int z 
Returns: 
adds point (x, y, z) to points and increment points.lastcol
if points is full, should call grow on points
====================*/
void add_point( struct matrix * points, double x, double y, double z) {
  
  if ( points->lastcol == points->cols )
    grow_matrix( points, points->lastcol + 100 );

  points->m[0][points->lastcol] = x;
  points->m[1][points->lastcol] = y;
  points->m[2][points->lastcol] = z;
  points->m[3][points->lastcol] = 1;

  points->lastcol++;
}

/*======== void add_edge() ==========
Inputs:   struct matrix * points
          int x0, int y0, int z0, int x1, int y1, int z1
Returns: 
add the line connecting (x0, y0, z0) to (x1, y1, z1) to points
should use add_point
====================*/
void add_edge( struct matrix * points, 
	       double x0, double y0, double z0, 
	       double x1, double y1, double z1) {
  add_point( points, x0, y0, z0 );
  add_point( points, x1, y1, z1 );
}

/*======== void draw_lines() ==========
Inputs:   struct matrix * points
         screen s
         color c 
Returns: 
Go through points 2 at a time and call draw_line to add that line
to the screen
====================*/
void draw_lines( struct matrix * points, screen s, color c) {

  int i;
 
  if ( points->lastcol < 2 ) {
    
    printf("Need at least 2 points to draw a line!\n");
    return;
  }

  for ( i = 0; i < points->lastcol - 1; i+=2 ) {

    draw_line( points->m[0][i], points->m[1][i], 
	       points->m[0][i+1], points->m[1][i+1], s, c);
  } 	       
}

void draw_line(int x0, int y0, int x1, int y1, screen s, color c) {
 
  int x, y, d, dx, dy;

  x = x0;
  y = y0;
  
  //swap points so we're always draing left to right
  if ( x0 > x1 ) {
    x = x1;
    y = y1;
    x1 = x0;
    y1 = y0;
  }

  //need to know dx and dy for this version
  dx = (x1 - x) * 2;
  dy = (y1 - y) * 2;

  //positive slope: Octants 1, 2 (5 and 6)
  if ( dy > 0 ) {

    //slope < 1: Octant 1 (5)
    if ( dx > dy ) {
      d = dy - ( dx / 2 );
  
      while ( x <= x1 ) {
	plot(s, c, x, y);

	if ( d < 0 ) {
	  x = x + 1;
	  d = d + dy;
	}
	else {
	  x = x + 1;
	  y = y + 1;
	  d = d + dy - dx;
	}
      }
    }

    //slope > 1: Octant 2 (6)
    else {
      d = ( dy / 2 ) - dx;
      while ( y <= y1 ) {

	plot(s, c, x, y );
	if ( d > 0 ) {
	  y = y + 1;
	  d = d - dx;
	}
	else {
	  y = y + 1;
	  x = x + 1;
	  d = d + dy - dx;
	}
      }
    }
  }

  //negative slope: Octants 7, 8 (3 and 4)
  else { 

    //slope > -1: Octant 8 (4)
    if ( dx > abs(dy) ) {

      d = dy + ( dx / 2 );
  
      while ( x <= x1 ) {

	plot(s, c, x, y);

	if ( d > 0 ) {
	  x = x + 1;
	  d = d + dy;
	}
	else {
	  x = x + 1;
	  y = y - 1;
	  d = d + dy + dx;
	}
      }
    }

    //slope < -1: Octant 7 (3)
    else {

      d =  (dy / 2) + dx;

      while ( y >= y1 ) {
	
	plot(s, c, x, y );
	if ( d < 0 ) {
	  y = y - 1;
	  d = d + dx;
	}
	else {
	  y = y - 1;
	  x = x + 1;
	  d = d + dy + dx;
	}
      }
    }
  }
}

