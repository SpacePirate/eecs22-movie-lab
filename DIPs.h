/**************************************************************************************************/
/* DIPs.h: header file for DIP module                                                             */
/* author: Deukkwon Yoon                                                                          */
/**************************************************************************************************/

#ifndef DIPS_H
#define DIPS_H

#include "Image.h"
#define MAX_COLOR 16

/* change color image to black & white */
void BlackNWhite(IMAGE *image);

/* flip image vertically */
void VFlip(IMAGE *image);

/* mirror image horizontally */
void HMirror(IMAGE *image);

/* edge detection */
void Edge(IMAGE *image);
              
/* Posterize the image */
void Posterize(IMAGE *image);

IMAGE *Resize(unsigned int percentage, IMAGE *image);

/*Mandelbrot*/
IMAGE *Mandelbrot(unsigned int W, unsigned int H, unsigned int max_iteration, long double zoom);

#endif /* DIPS_H */