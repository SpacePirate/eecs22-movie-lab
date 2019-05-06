/******************************************************************************/ 
/* Image.h: source code for basic image manipulations                         */
/*                                                                            */ 
/* 11/20/13 Che-Wei Chang: Union                                              */
/* 11/07/11 Weiwei Chen: modified for EECS22 assignment5 Fall2011             */
/* 10/20/11 Weiwei Chen: initial solution version                             */
/*                       for EECS22 assignment4 FAll2011                      */
/******************************************************************************/ 

#include "Movie.h"

/* allocate the memory space for the movie */
/* and the memory space for the frame list. */
/* return the pointer to the movie */
MOVIE *CreateMovie(unsigned int nFrames, unsigned int W, unsigned int H)
{
    MOVIE *movie;
    
    assert(nFrames > 0);
    assert(W > 0);
    assert(H > 0);
    
    if (!(movie = (MOVIE *)malloc(sizeof(MOVIE))))
    { return NULL; }
    
    movie->Width = W;
    movie->Height = H;
    movie->NumFrames = nFrames;
    movie->Frames = NewImageList();
    movie->Frames->Length = nFrames;
    assert(movie->Frames);
    
    return movie;
}

/*release the memory space for the frames and the frame list. */
/*release the memory space for the movie. */
void DeleteMovie(MOVIE *movie)
{
    printf("Deleting Movie\n");
    assert(movie);
    DeleteImageList(movie->Frames);
    movie->Frames = NULL;
    free(movie);
}

/* convert a YUV image into a RGB image */
void YUV2RGBImage(IMAGE *image)
{
    IMAGE *temp;
    
    int x, y;
    int C = 0;
    int D = 0;
    int E = 0;
    int tmpR = 0;
    int tmpG = 0;
    int tmpB = 0;
    
    assert(image);
    unsigned int h; 
    unsigned int w;  
    
    h = image->Height;
    w = image->Width;
    
    temp = CreateImage(w, h);
    assert(temp);
    
    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w; x++)
        {
            SetPixelY(temp, x, y, GetPixelY(image, x, y));
            SetPixelU(temp, x, y, GetPixelU(image, x, y));
            SetPixelV(temp, x, y, GetPixelV(image, x, y));
        }
    }

    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w; x++)
        {
            C = GetPixelY(temp, x, y) - 16;
            D = GetPixelU(temp, x, y) - 128;
            E = GetPixelV(temp, x, y) - 128;
            
            tmpR = (298 * C + 409 * E + 128) >> 8;
            tmpG = (298 * C - 100 * D - 208 * E + 128) >> 8;
            tmpB = (298 * C + 516 * D + 128) >> 8;
            
            SetPixelR(image, x, y, ((tmpR > 255) ? 255 : (tmpR < 0) ? 0 : tmpR));
            SetPixelG(image, x, y, ((tmpG > 255) ? 255 : (tmpG < 0) ? 0 : tmpG));
            SetPixelB(image, x, y, ((tmpB > 255) ? 255 : (tmpB < 0) ? 0 : tmpB));
        }
    }
    DeleteImage(temp);
}

/* convert a RGB image into a YUV image */
void RGB2YUVImage(IMAGE *image)
{
    IMAGE *temp;
    
    int x, y;
    int R = 0;
    int G = 0;
    int B = 0;
    int tmpY = 0;
    int tmpU = 0;
    int tmpV = 0;
    
    assert(image);
    unsigned int h; 
    unsigned int w;  
    
    h = image->Height;
    w = image->Width;
    
    temp = CreateImage(w, h);
    assert(temp);
    
    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w; x++)
        {
            SetPixelR(temp, x, y, GetPixelR(image, x, y));
            SetPixelG(temp, x, y, GetPixelG(image, x, y));
            SetPixelB(temp, x, y, GetPixelB(image, x, y));
        }
    }
    
    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w; x++)
        {
            R = GetPixelR(temp, x, y);
            G = GetPixelG(temp, x, y);
            B = GetPixelB(temp, x, y);

            tmpY = ((66 * R + 129 * G + 25 * B + 128) >> 8) + 16;
            tmpU = ((-38 * R - 74 * G + 112 * B + 128) >> 8) + 128;
            tmpV = ((112 * R- 94 * G - 18 * B + 128) >> 8) + 128;
            
            SetPixelY(image, x, y, ((tmpY > 255) ? 255 : (tmpY < 0) ? 0 : tmpY));
            SetPixelU(image, x, y, ((tmpU > 255) ? 255 : (tmpU < 0) ? 0 : tmpU));
            SetPixelV(image, x, y, ((tmpV > 255) ? 255 : (tmpV < 0) ? 0 : tmpV));
        }
    }
    DeleteImage(temp);
}

/* EOF Movie.c */
