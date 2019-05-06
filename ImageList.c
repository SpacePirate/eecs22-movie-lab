/******************************************************************************/
/* ImageList.c:                                                               */
/* author: Deukkwon Yoon                                                      */
/******************************************************************************/

#include "ImageList.h"

/* allocate a new image list */
ILIST *NewImageList(void)
{
    ILIST *l;
    
    if(!(l = (ILIST *)malloc(sizeof(ILIST))))
    { return NULL; }
    
    l->Length = 0;
    l->First = NULL;
    l->Last = NULL;
    
    return l;
}

/* delete a image list (and all entries) */
void DeleteImageList(ILIST *l)
{
    IENTRY *e, *n;
    IMAGE *image;
    assert(l);
    
    /* Delete entry by entry from first to last */
    e = l->First;
    while(e)
    {
        n = e->Next;
        image = DeleteImageEntry(e);
        DeleteImage(image);
        e = n;
    }
    
    free(l);
}

IENTRY *NewImageEntry(IMAGE *image)
{
    IENTRY *e;
    if(!(e = (IENTRY *)malloc(sizeof(IENTRY))))
    { return NULL; }
    
    e->List = NULL;
    e->Next = NULL;
    e->Prev = NULL;
    e->Image = image;
    
    return e;
}

IMAGE *DeleteImageEntry(IENTRY *e)
{
    IMAGE *image;
    
    assert(e);
    image = e->Image;

    free(e);
    return image;
}

/* insert a frame into a list at the end*/
void AppendImage(ILIST *l, IMAGE *image)
{
    IENTRY *e = NULL;
    
    assert(l);
    assert(image);

    e = NewImageEntry(image);
    assert(e);
    e->List = l;
    
    if(l->Last)
    {
        e->Next = NULL;
        e->Prev = l->Last;
        l->Last->Next = e;
        l->Last = e;
    }
    else
    {
        e->Next = NULL;
        e->Prev = NULL;
        l->First = e;
        l->Last = e;
    }
    l->Length++;
}

/* reverse an image list */
void ReverseImageList(ILIST *l)
{
    IENTRY *e;
    IENTRY *p;
    IENTRY *n;
    IENTRY *first;
    IENTRY *last;
    
    assert(l);
    
    last = l->Last;
    first = l->First;
    
    l->First = last;
    l->Last = first;
    
    if (last == first)
    { /* Do nothing this list has one frrame */ }
    else
    {
        /* Go through the list backwards*/
        e = last;
        while(e)
        {
            p = e->Prev;
            n = e->Next;

            /* This is the last entry (now first) */
            if(!n)
            {
                e->Next = p;
                e->Prev = NULL;
            }
            
            /* This is the first entry (now last) */
            else if (!p)
            {
                e->Next = NULL;
                e->Prev = n;
            }
            
            else
            {
                e->Next = p;
                e->Prev = n;
            }
        
            e = p;
        }
    }
}

IMAGE *RemoveFirstEntry(ILIST *l)
{
    IENTRY *e = NULL;
    assert(l);
    assert(l->First); /* List must contain some entries */
    e = l->First;
    l->First = e->Next;
    if (l->First)
    {
        l->First->Prev = NULL;
    }
    else
    {
        assert(l->Last == e);
        l->Last = NULL;
    }
    l->Length--;
    return DeleteImageEntry(e);
}

IMAGE *RemoveLastEntry(ILIST *l)
{
    IENTRY *e = NULL;
    assert(l);
    assert(l->Last); /* List must contain some entries */
    e = l->Last;
    l->Last = e->Prev;
    if (l->Last)
    {
        l->Last->Next = NULL;
    }
    else
    {
        assert(l->First == e);
        l->First = NULL;
    }
    l->Length--;
    return DeleteImageEntry(e);
}

/*Crop frames from the list starting from SFrame to EFrame*/
void CropImageList(ILIST *l, unsigned int SFrame, unsigned int EFrame)
{
    unsigned int i;
    unsigned int num;
    num = l-> Length;
    assert(l);
    IMAGE *image;
    
    /* Check upper bound of cropping parameter */
    assert(EFrame <= l->Length);
    assert(EFrame > SFrame);
    
    for (i = 0; i < SFrame; i++)
    { 
        image = RemoveFirstEntry(l);
        DeleteImage(image);
        l->Length--;
    }
    for(i = num; i > EFrame; i--)
    {
        image = RemoveLastEntry(l);
        DeleteImage(image);
        l->Length--;
    }
}

/*Resize the images in the list*/
void ResizeImageList(ILIST *l, unsigned int percentage)
{
    IMAGE *image;
    IMAGE *new_image;
    IENTRY *e = NULL;
    IENTRY *n = NULL;
    
    assert(l);
    e = l->First;
    while(e)
    {
        n = e->Next;
        image = e->Image;
        new_image = Resize(percentage, image);
        assert(new_image);
        e->Image = new_image;
        DeleteImage(image);
        
        e = n;
    }
}

/*Fast forward the video*/
unsigned int FastImageList(ILIST *l, unsigned int ff_factor)
{
    IENTRY *e = NULL;
    IENTRY *n = NULL;
    IENTRY *p = NULL;
    IMAGE *image;

    assert(l);
    unsigned int frames = l->Length;
    unsigned int f_count = 0;
    unsigned int new_frame_num = 0;
    
    e = l->First;
    while(e)
    {
        n = e->Next;
        if (f_count % ff_factor == 0)
        {
            /* Keep this frame */
            new_frame_num++;
        }
        else
        {
            if (f_count == 0)
            {
                image = RemoveFirstEntry(l);
                DeleteImage(image);
            }
            else if (f_count == frames - 1)
            {
                image = RemoveLastEntry(l);
                DeleteImage(image);
            }
            else
            {
                p = e->Prev;
                e->Prev->Next = n;
                e->Next->Prev = p;
                image = DeleteImageEntry(e);
                assert(image);
                DeleteImage(image);
                l->Length--;
            }
        }
        e = n;
        f_count++;
    }
    
    return new_frame_num;
}


/* EOF ImageList.c */