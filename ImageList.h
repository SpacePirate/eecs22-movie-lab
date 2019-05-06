/******************************************************************************/
/* ImageList.h: header file for ImageList module                              */
/* author: Deukkwon Yoon                                                      */
/*                                                                            */
/******************************************************************************/

#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Image.h"
#include "DIPs.h"

typedef struct ImageList ILIST;
typedef struct ImageEntry IENTRY;

struct ImageEntry{
    ILIST *List; /* pointer to the list which this entry belongs to */
    IENTRY *Next; /* pointer to the next entry, or NULL */
    IENTRY *Prev; /* pointer to the previous entry, or NULL */
    IMAGE *Image; /* pointer to the struct for the image */
};

struct ImageList{
    unsigned int Length; /* Length of the list */
    IENTRY *First; /* pointer to the first entry, or NULL */
    IENTRY *Last; /* pointer to the last entry, or NULL */
};

/* allocate a new image list */
ILIST *NewImageList(void);

/* delete a image list (and all entries) */
void DeleteImageList(ILIST *l);

IENTRY *NewImageEntry(IMAGE *image);

IMAGE *DeleteImageEntry(IENTRY *e);

IMAGE * RemoveFirstEntry(ILIST *l);
IMAGE * RemoveLastEntry(ILIST *l);

/* insert a frame into a list at the end*/
void AppendImage(ILIST *l, IMAGE *image);

/* reverse an image list */
void ReverseImageList(ILIST *l);

/*Crop frames from the list starting from SFrame to EFrame*/
void CropImageList(ILIST *l, unsigned int SFrame, unsigned int EFrame);

/*Resize the images in the list*/
void ResizeImageList(ILIST *l, unsigned int percentage);

/*Fast forward the video*/
unsigned int FastImageList(ILIST *l, unsigned int ff_factor);

#endif