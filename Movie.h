/**************************************************************************************************/
/* Image.h: header file for basic image manipulations                                             */
/* author: Deukkwon Yoon                                                                          */
/**************************************************************************************************/
#ifndef MOVIE_H
#define MOVIE_H

#include "ImageList.h"

/* the structure for MOVIE */
typedef struct{
    unsigned int Width;     /* movie frame width */
    unsigned int Height;    /* movie frame height */
    unsigned int NumFrames; /* total number of frames */
    ILIST *Frames;          /* the pointer to the frame list */
}MOVIE;

/* allocate the memory space for the movie */
/* and the memory space for the frame list. */
/* return the pointer to the movie */
MOVIE *CreateMovie(unsigned int nFrames, unsigned int W, unsigned int H);

/*release the memory space for the frames and the frame list. */
/*release the memory space for the movie. */
void DeleteMovie(MOVIE *movie);

/* convert a YUV image into a RGB image */
void YUV2RGBImage(IMAGE *image);
/* convert a RGB image into a YUV image */
void RGB2YUVImage(IMAGE *image);

#endif
