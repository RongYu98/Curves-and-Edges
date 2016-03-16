#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

int main( int argc, char **argv ) {

  screen s;
  struct matrix *edges;
  struct matrix *transform;

  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);

  color c;
  c.red = 0;
  c.green = 45;
  c.blue = 200;

  /*
  if ( argc == 2 )
    parse_file( argv[1], transform, edges, s );
  else
    parse_file( "stdin", transform, edges, s );
  */

  //printf("1\n");


  //generate_curve_coefs(150, 150, 150, 50, 350 150 350 300
  
  /*
  add_circle( edges, 250,250, 50, .1);

  draw_lines(edges, s, c);
  save_extension(s, "lines.png");
  display(s);
  print_matrix(edges);
  */
  
  //transform = make_hermite();
  //transform = make_bezier();
  //transform = generate_curve_coefs(1,6,6,5,1);
  //print_matrix(transform);
  free_matrix( transform );
  free_matrix( edges );
}  
