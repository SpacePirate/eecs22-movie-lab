/*********************************************************************/
/* MovieLab.c: homework assignment #5, for EECS 22,    Fall 2011             */
/*                                                                                                                                     */
/* History:                                                                                                                    */
/* 10/30/11 Weiwei Chen    updated for EECS22 assignment5 FAll2011         */
/* 10/30/11 Weiwei Chen    : initial template version                                     */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "Image.h"
#include "DIPs.h"
#include "Movie.h"

/*read the movie frames from the input file */
int ReadMovie(const char *fname, int nFrame,
              unsigned int W, unsigned H, MOVIE *movie);

IMAGE* ReadOneFrame(const char *fname, int nFrame, unsigned int W, unsigned H);

/*save the movie frames to the output file */
int SaveMovie(const char *fname, MOVIE *movie);

/* Print the command line parameter usage of the program */
void PrintUsage();

/* type define the function pointer to the DIP function */
typedef void MOP_F(IMAGE *image);

/* the function for perform DIP operations on the movie */
void Movie_DIP_Operation(MOVIE *movie, MOP_F *MovieOP);

int MandelbrotMovie(int nFrame, /*number of frames in the movie*/
                    unsigned int W, /*the width of the movie*/
                    unsigned H, /*the height of the movie*/
                    MOVIE *movie /* the pointer to the output movie*/);

int main(int argc, char *argv[])
{
    MOVIE *movie;
    ILIST *l;
    
    unsigned int x = 1;
    char *fin = NULL, *fout = NULL;
    unsigned int bw = 0;
    unsigned int vflip = 0;
    unsigned int hmirror = 0;
    unsigned int edge = 0;
    unsigned int poster = 0;
    unsigned int rvs = 0;
    unsigned int mbrot = 0;
    int cs = -1;
    int ce = -1;
    unsigned int fastf = 0;
    int sizef = -1;
    
    unsigned int nFrames = 0;
    unsigned int W = 0;
    unsigned int H = 0;
    unsigned int w_new, h_new = 0;
    
    int new_frame_num = 0;
    
    /*entering while loop to check options entered*/
    while(x < argc)
    {
        /* Mandelbrot sequences */
        if(0 == strcmp(&argv[x][0], "-m"))
        {
            mbrot = 1;
            x += 1;
            continue;
        }/*fi*/
        
        /* specify input video */
        if(0 == strcmp(&argv[x][0], "-i")) 
        {
            if(x < argc - 1)
            {
                fin = (char *)malloc(sizeof(char) * (strlen(&argv[x + 1][0]) + 
                                                     strlen(".yuv") + 1));
                strcpy(fin, argv[x + 1]);
                strcat(fin, ".yuv");
            }/*fi*/
            else
            {
                printf("Missing argument for input name!");
                free(fin);
                free(fout);
                return 5;
            }/*esle*/
            x += 2;
            continue;
        }/*fi*/

        /* specify output video */
        if(0 == strcmp(&argv[x][0], "-o")) 
        {
            if(x < argc - 1)
            {
                fout = (char *)malloc(sizeof(char) * (strlen(&argv[x + 1][0]) + 
                                                      strlen(".yuv") + 1));
                strcpy(fout, argv[x + 1]);
                strcat( fout, ".yuv");
            }/*fi*/
            else
            {
                printf("Missing argument for output name!");
                free(fin);
                free(fout);
                return 5;
            }/*esle*/
            x += 2;
            continue;
        }/*fi*/
        
        /* help option */
        if(0 == strcmp(&argv[x][0], "-h")) 
        { 
            PrintUsage();
            free(fin);
            free(fout);
            return 0;
        }/*fi*/
        
        /* Determine number of frames */
        if (0 == strcmp(&argv[x][0], "-f"))
        {
            if(x < argc - 1)
            {
                /* input is correct */
                if(sscanf(argv[x + 1], "%d", &nFrames) == 1)
                {
                    /* number of frames stored in nFrames */
                }
                else
                {
                    printf("Input format error!");
                    free(fin);
                    free(fout);
                    return 5;
                }
            }/*fi*/
            else
            {
                printf("Missing argument for frame number!");
                free(fin);
                free(fout);
                return 5;
            }/*esle*/
            
            x += 2;
            continue;
        }
        
        /* Determine frame size */
        if (0 == strcmp(&argv[x][0], "-s"))
        {
            if(x < argc - 1)
            {
                /* input is correct */
                if(sscanf(argv[x + 1], "%dx%d", &W, &H) == 2)
                {
                    /* the width is stored in W */
                    /* the height is stored in H */
                }
                /*input format error*/
                else
                {
                    printf("Input format error!");
                    free(fin);
                    free(fout);
                    return 5;
                }
            }/*fi*/
            else
            {
                printf("Missing argument for frame number!");
                free(fin);
                free(fout);
                return 5;
            }/*esle*/
            
            x += 2;
            continue;
        }
        
        /* Determine cut index */
        if (0 == strcmp(&argv[x][0], "-cut"))
        {
            if(x < argc - 1)
            {
                /* input is correct */
                if(sscanf(argv[x + 1], "%d-%d", &cs, &ce) == 2)
                {
                    /* the cut start is stored in cs */
                    /* the cut end is stored in ce */
                }
                
                /*input format error*/
                else
                {
                    printf("Input format error!");
                    free(fin);
                    free(fout);
                    return 5;
                }
            }/*fi*/
            else
            {
                printf("Missing argument for frame number!");
                free(fin);
                free(fout);
                return 5;
            }/*esle*/
            
            x += 2;
            continue;
        }

        /* Determine resize dimension */
        if (0 == strcmp(&argv[x][0], "-resize"))
        {
            if(x < argc - 1)
            {
                /* input is correct */
                if(sscanf(argv[x + 1], "%d", &sizef) == 1)
                {
                    /* the size factor is stored in sizef */
                }
                
                /*input format error*/
                else
                {
                    printf("Input format error!");
                    free(fin);
                    free(fout);
                    return 5;
                }
            }/*fi*/
            else
            {
                printf("Missing argument for frame number!");
                free(fin);
                free(fout);
                return 5;
            }/*esle*/
            
            x += 2;
            continue;
        }
        
        /* Determine fast forward factor */
        if (0 == strcmp(&argv[x][0], "-fast"))
        {
            if(x < argc - 1)
            {
                /* input is correct */
                if(sscanf(argv[x + 1], "%d", &fastf) == 1)
                {
                    /* the speed factor is stored in fastf */
                }
                
                /*input format error*/
                else
                {
                    printf("Input format error!");
                    free(fin);
                    free(fout);
                    return 5;
                }
            }/*fi*/
            else
            {
                printf("Missing argument for frame number!");
                free(fin);
                free(fout);
                return 5;
            }/*esle*/
            
            x += 2;
            continue;
        }
        
        /* Check DIP on the movie */
        if (0 == strcmp(&argv[x][0], "-bw"))
        { 
            bw = 1; 
            x += 1;
            continue;
        }
        if (0 == strcmp(&argv[x][0], "-vflip"))
        {
            vflip = 1; 
            x += 1;
            continue;
        }
        if (0 == strcmp(&argv[x][0], "-hmirror"))
        {
            hmirror = 1;
            x += 1;
            continue;
        }
        if (0 == strcmp(&argv[x][0], "-edge"))
        {
            edge = 1;
            x += 1;
            continue;
        }
        if (0 == strcmp(&argv[x][0], "-poster"))
        {
            poster = 1;
            x += 1;
            continue;
        }
        if (0 == strcmp(&argv[x][0], "-rvs"))
        {
            rvs = 1;
            x += 1;
            continue;
        }
        if (0 == strcmp(&argv[x][0], "-mbrot"))
        {
            mbrot = 1;
            x += 1;
            continue;
        }

        x++;
    }/*elihw*/

    if(!fout){
        printf("Missing argument for output name!\n");
        PrintUsage();
        free(fin);
        free(fout);
        return 5;
    }
        
    movie = CreateMovie(nFrames, W, H);

    /* Create Mandelbrot movie */
    if (mbrot == 1)
    {
        MandelbrotMovie(nFrames, W, H, movie);
        printf("Operation Mandelbrot is done!\n");
    }
    
    /* Perform DIP on the movie */
    else
    {
        /* Input doesn't matter for Mandelbrot */
        if(!fin){
            printf("Missing argument for input name!\n");
            PrintUsage();
            free(fin);
            free(fout);
            return 5;
        }
        
        ReadMovie(fin, nFrames, W, H, movie);
        l = movie->Frames;

        if (bw == 1)
        {
            Movie_DIP_Operation(movie, BlackNWhite);
            printf("Operation BlackNWhite is done!\n");
        }
        if (vflip == 1)
        {
            Movie_DIP_Operation(movie, VFlip);
            printf("Operation VFlip is done!\n");
        }
        if (hmirror == 1)
        {
            Movie_DIP_Operation(movie, HMirror);
            printf("Operation HMirror is done!\n");
        }
        if (edge == 1)
        {
            Movie_DIP_Operation(movie, Edge);
            printf("Operation Edge is done!\n");
        }
        if (poster == 1)
        {
            Movie_DIP_Operation(movie, Posterize);
            printf("Operation Posterize is done!\n");
        }

        if (sizef != -1)
        {
            assert(sizef > 1 && sizef <= 100);
            ResizeImageList(l, sizef);
            w_new = W * (double)(sizef / 100.00);
            h_new = H * (double)(sizef / 100.00);
            movie->Width = w_new;
            movie->Height = h_new;
            printf("Operation Resize is done! New Size is: %dx%d\n",
                   w_new, h_new);
        }

        if (cs != -1 && ce != -1)
        {
            CropImageList(l, cs, ce);
            movie->NumFrames = ce - cs;
            printf("Operation Frame Cropping is done!\n");
        }

        if (fastf > 1)
        {
            new_frame_num = FastImageList(l, fastf);
            printf("Operation Fast Forward is done! Number of frames = %d\n",
                   new_frame_num);
            movie->NumFrames = new_frame_num;
        }
        if (rvs == 1)
        {
            ReverseImageList(l);
            printf("Operation ReverseMovie is done!\n");
        }
    }

    SaveMovie(fout, movie);
    
    free(fin);
    free(fout);
    fin = NULL;
    fout = NULL;
    
    return 0;
}

void PrintUsage()
{
    printf(
        "\nFormat on command line is:\n"
        "MovieLab [option]\n"
        "-i [file_name]    to provide the input file name\n"
        "-o [file_name]    to provide the    output file name\n"
        "-f [no_frames]    to determine how many frames desired "
                           "in the input stream\n"
        "-s [WidthxHeight] to set resolution of the input stream "
                           "(widthxheight)\n"
        "-j                to generate the movie with JuliaSet sequences\n"
        "-bw               to activate the conversion to black and white\n"
        "-vflip            to activate vertical flip\n"
        "-hmirror          to activate horizontal flip\n"
        "-edge             to activate edge filter\n"
        "-poster           to activate posterize filter\n"
        "-cut [Start-End]  to crop the frame from the video from frame Start "
                           "to frame End\n"  
        "-resize [factor]  to resize the video with the provided factor "
                           "[1-100]\n"
        "-fast   [factor]  to fast forward the video with the provided factor "
                           "[1+]\n"
        "-rvs              to reverse the frame order of the input stream\n"
        "-h                to show this usage information\n"
    );
}

IMAGE* ReadOneFrame(const char *fname, int nFrame, unsigned int W, unsigned H)
{
    /*defining local variables*/
    FILE *file;
    
    unsigned int x, y;
    unsigned char ch;
    IMAGE* image;

    /*checking error*/
    assert(fname);
    assert(nFrame >= 0);

    image = CreateImage(W, H);
    assert(image);

    /*opening file stream*/
    file = fopen(fname, "r");
    assert(file);

    /*find desired frame*/
    fseek(file, 1.5 * nFrame * W * H, SEEK_SET);

    for(y = 0; y < H; y ++){
        for(x = 0; x < W; x ++){
            ch = fgetc(file);
            SetPixelY(image, x, y, ch);
            }/*rof*/
    }

    for(y = 0; y < H ; y += 2){
        for(x = 0; x < W ; x += 2){
            ch = fgetc(file);
            SetPixelU(image, x, y, ch);
            SetPixelU(image, x + 1, y, ch);
            SetPixelU(image, x, y + 1, ch);
            SetPixelU(image, x + 1, y + 1, ch);
        }
    }

    for(y = 0; y < H ; y += 2){
        for(x = 0; x < W ; x += 2){
            ch = fgetc(file);
                SetPixelV(image, x, y, ch);
                SetPixelV(image, x + 1, y, ch);
                SetPixelV(image, x, y + 1, ch);
                SetPixelV(image, x + 1, y + 1, ch);
        }
    }
    
    /*checking for error*/
    assert(ferror(file) == 0) ;

    /*closing stream and terminating*/
    fclose(file);
    file = NULL;
    
    return image;
}

/*read the movie frames from the input file */
int ReadMovie(const char *fname, int nFrame,
              unsigned int W, unsigned H, MOVIE *movie)
{
    IMAGE *image;
    assert(movie);
    int n;
    for (n = 0; n < nFrame; n++ )
    {
        image = ReadOneFrame(fname, n, W, H);
        assert(image);
        YUV2RGBImage(image);
        AppendImage(movie->Frames, image);
    }
    
    return 0;
}
              
/*save the movie frames to the output file */
int SaveMovie(const char *fname, MOVIE *movie)
{
    int i; 
    int x, y;
    FILE *file;
    IENTRY *currFrame;
    IMAGE *image;

    assert(movie);
     /*opening file stream*/
    if(!(file = fopen(fname, "w"))){ return 1; }
     
    i = 0 ;
    printf("start SaveMovie, total frame %d\n", movie->NumFrames); 
    currFrame = movie->Frames->First;
    while(currFrame)
    {
        image = currFrame->Image;
        RGB2YUVImage(image);
        
        for(y = 0; y < movie->Height; y ++){ 
            for(x = 0; x < movie->Width; x ++){
                fputc(GetPixelY(image, x, y), file);
            }
        }
        for(y = 0; y < movie->Height; y += 2){ 
            for(x = 0; x < movie->Width; x += 2){
                fputc(GetPixelU(image, x, y), file);
            }
        }
        for(y = 0; y < movie->Height; y += 2){ 
            for(x = 0; x < movie->Width; x += 2){
                fputc(GetPixelV(image, x, y), file);
            }
        }
        currFrame = currFrame->Next;     
        i++;
    }

    fclose(file);
    file = NULL;
    
    DeleteMovie(movie);
    
    printf("The movie file %s has been written successfully! \n", fname);
    printf("%d frames are written to the file %s in total \n", i, fname); 
    return 0;
}

/* the function for perform DIP operations on the movie*/
void Movie_DIP_Operation(MOVIE *movie, MOP_F *MovieOP)
{
    IENTRY *e = NULL;
    IMAGE *image;
    
    assert(movie);
    e = movie->Frames->First;
    while(e)
    {
        image = e->Image;
        MovieOP(image);
        e = e->Next;
    }
}

int MandelbrotMovie(int nFrame, /*number of frames in the movie*/
                    unsigned int W, /*the width of the movie*/
                    unsigned H, /*the height of the movie*/
                    MOVIE *movie /* the pointer to the output movie*/)
{
    IMAGE *image;
    ILIST *l;
    int n;

    assert(movie);
    l = movie->Frames;
    for(n = 0; n < nFrame; n++)
    {
        image = Mandelbrot(W, H, 2000, pow(1.17, n));
        AppendImage(l, image);
    }
    
    return 0;
}