/**************************************************************************************************/
/* DIPs.c: DIP module                                                                             */
/* author: Deukkwon Yoon                                                                          */
/**************************************************************************************************/

#include "DIPs.h"

/* change color image to black & white */
void BlackNWhite(IMAGE *image)
{
    int x, y;
    unsigned char temp;
    
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    assert(image);
    for( y = 0; y < h; y++ )
    {
        for( x = 0; x < w; x++ )
        {
            temp = (GetPixelR(image, x, y) +
                    GetPixelG(image, x, y) +
                    GetPixelB(image, x, y))/3;
            
            SetPixelR(image, x, y, temp);
            SetPixelG(image, x, y, temp);
            SetPixelB(image, x, y, temp);
        }
    }
}

/* flip image vertically */
void VFlip(IMAGE *image)
{
    int x, y;
    unsigned char r, g, b;
    
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    assert(image);
    for (y = 0; y < h/2; y++)
    {
        for (x = 0; x < w; x++)
        {
            r = GetPixelR(image, x, h - 1 - y);
            g = GetPixelG(image, x, h - 1 - y);
            b = GetPixelB(image, x, h - 1 - y);

            SetPixelR(image, x, h - 1 - y, GetPixelR(image, x, y));
            SetPixelG(image, x, h - 1 - y, GetPixelG(image, x, y));
            SetPixelB(image, x, h - 1 - y, GetPixelB(image, x, y));

            SetPixelR(image, x, y, r);
            SetPixelG(image, x, y, g);
            SetPixelB(image, x, y, b);
        }
    }
}

/* mirror image horizontally */
void HMirror(IMAGE *image)
{
    int x, y;
    
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    assert(image);
    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w / 2; x++)
        {
            SetPixelR(image, x, y, GetPixelR(image, w - 1 - x, y));
            SetPixelG(image, x, y, GetPixelG(image, w - 1 - x, y));
            SetPixelB(image, x, y, GetPixelB(image, w - 1 - x, y));
        }
    }
}   

/* edge detection */
void Edge(IMAGE *image)
{
    int x, y, m, n, a, b;
    int tmpR = 0;
    int tmpG = 0;
    int tmpB = 0;
    unsigned int w;
    unsigned int h;
    IMAGE *temp;

    assert(image);
    w  = image->Width;
    h = image->Height;

    temp = CreateImage(w, h);
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
            for (n = -1; n <= 1; n++)
                for (m = -1; m <= 1; m++)
                {
                    a = x + m;
                    b = y + n;
                    if (a > w - 1)
                        a = w - 1;
                    if (a < 0)
                        a = 0;
                    if (b > h - 1)
                        b = h - 1;
                    if (b < 0)
                        b = 0;

                    if ((n==0)&&(m==0))
                    {
                        tmpR += 8*GetPixelR(temp, a, b);
                        tmpG += 8*GetPixelG(temp, a, b);
                        tmpB += 8*GetPixelB(temp, a, b);
                    }
                    else
                    {
                        tmpR -= GetPixelR(temp, a, b);
                        tmpG -= GetPixelG(temp, a, b);
                        tmpB -= GetPixelB(temp, a, b);
                    }
                }
        SetPixelR(image, x, y, ((tmpR > 255) ? 255 : (tmpR < 0) ? 0 : tmpR));
        SetPixelG(image, x, y, ((tmpG > 255) ? 255 : (tmpG < 0) ? 0 : tmpG));
        SetPixelB(image, x, y, ((tmpB > 255) ? 255 : (tmpB < 0) ? 0 : tmpB));

        tmpR = tmpG = tmpB = 0;
        }
    }
    DeleteImage(temp);
}

/* Posterize the image */
void Posterize(IMAGE *image)
{
    /*
    0x0F is a hexadecimal representation of decimal intger 15
    In binary, 00001111.
    c = c & 0x0F extracts lowest 4 bits from char c to 0
    c = c | 0x0F sets lowest 4 bits of char c to 1
    */
    
    int x, y;
    unsigned char r, g, b;
    int bits_to_0, bits_to_1;
    
    unsigned int pbits = 6;
    unsigned int w = image->Width;
    unsigned int h = image->Height;
    
    assert(image);

    bits_to_0 = (1 << 8) - (1 << pbits); /* 1's nth bit 0's */
    bits_to_1 = (1 << (pbits - 1)) - 1;  /* 0's nth -1 bit 1's */

    for( y = 0; y < h; y++ )
    {
        for( x = 0; x < w; x++ )
        {
            /* Get RGB from the original image */
            r = GetPixelR(image, x, y);
            g = GetPixelG(image, x, y);
            b = GetPixelB(image, x, y);
        
            /* Assign values to the template */
            SetPixelR(image, x, y, ((r & bits_to_0) | bits_to_1));
            SetPixelG(image, x, y, ((g & bits_to_0) | bits_to_1));
            SetPixelB(image, x, y, ((b & bits_to_0) | bits_to_1));
        }    
    }
}

/* Resize */
IMAGE *Resize(unsigned int percentage, IMAGE *image)
{
    assert(image);
    
    unsigned int x, y, i;
    unsigned int x_new, y_new;
    unsigned char r, g, b;
    unsigned int avg_r, avg_g, avg_b;
    
    double p;
    p = (percentage / 100.00);
    
    unsigned int w;
    unsigned int h;
    w = image->Width;
    h = image->Height;
    
    unsigned int w_new = w * p;
    unsigned int h_new = h * p;
    
    /* Create template for result image */
    IMAGE *temp;
    temp = CreateImage(w_new, h_new);
    assert(temp);
    assert(percentage <= 500 && percentage >= 1);
    
    /* Enlarge image or same size */
    if (percentage >= 100)
    {
        for( y = 0; y < h; y++ )
        {
            for( x = 0; x < w; x++ )
            {
                /* Get RGB from the original image */
                r = GetPixelR(image, x, y);
                g = GetPixelG(image, x, y);
                b = GetPixelB(image, x, y);
                
                /* Use same RGB values for newly created pixels */
                for( y_new = y * p; y_new < (y + 1) * p; y_new++ )
                {
                    for( x_new = x * p; x_new < (x + 1) * p; x_new++ )
                    {
                        /* Assign values to the template */
                        SetPixelR(temp, x_new, y_new, r);
                        SetPixelG(temp, x_new, y_new, g);
                        SetPixelB(temp, x_new, y_new, b);
                    }
                }
            }
        }
    }
    
    /* Shrink image */
    else
    {
        for( y_new = 0; y_new < h_new; y_new++ )
        {
            for( x_new = 0; x_new < w_new; x_new++ )
            {
                /* Get average of bigger area in to smaller area */
                i = 0;
                avg_r = 0;
                avg_g = 0;
                avg_b = 0;

                /* Use same RGB values for newly created pixels */
                for( y = y_new/p; y < (y_new+1)/p; y++ )
                {
                    for( x = x_new/p; x < (x_new+1)/p; x++ )
                    {
                        /* Get RGB from the original image */
                        avg_r += GetPixelR(image, x, y);
                        avg_g += GetPixelG(image, x, y);
                        avg_b += GetPixelB(image, x, y);
                        i++;
                    }
                }
                /* Assign average values to the template */
                SetPixelR(temp, x_new, y_new, avg_r/i);
                SetPixelG(temp, x_new, y_new, avg_g/i);
                SetPixelB(temp, x_new, y_new, avg_b/i);
            }
        }
    }
    
    /* Note */
    /* Delete original image outside of this function */
    
    return temp;
}

/*Mandelbrot*/
IMAGE *Mandelbrot(unsigned int W, unsigned int H, unsigned int max_iteration,
                  long double zoom)
{
    IMAGE *image;
    unsigned int x, y;
    double x0, y0, x1, y1, num;
    unsigned int i;
    unsigned int color;
    unsigned char R, G, B;
    
    double offseth = -0.7;
    double offsetv = 0.27015;
    
    /* RGB color palette for Mandelbrot image */
    const unsigned char palette[MAX_COLOR][3] = {
        /*  r,   g,   b */
        {   0,   0,   0 },  /* 0, black */
        { 127,   0,   0 },  /* 1, brown */
        { 255,   0,   0 },  /* 2, red */
        { 255, 127,   0 },  /* 3, orange */
        { 255, 255,   0 },  /* 4, yellow */
        { 127, 255,   0 },  /* 5, light green */
        {   0, 255,   0 },  /* 6, green */
        {   0, 255, 127 },  /* 7, blue green */
        {   0, 255, 255 },  /* 8, turquoise */
        { 127, 255, 255 },  /* 9, light blue */
        { 255, 255, 255 }, /* 10, white */
        { 255, 127, 255 }, /* 11, pink */
        { 255,   0, 255 }, /* 12, light pink */
        { 127,   0, 255 }, /* 13, purple */
        {   0,   0, 255 }, /* 14, blue */
        {   0,   0, 127 }  /* 15, dark blue */
    };
    
    /* Create template for result image */
    image = CreateImage(W, H);
    assert(image);
    for( y = 0; y < W; y++ )
    {
        for( x = 0; x < H; x++ )
        {
            SetPixelY(image, x, y, 255);
            SetPixelU(image, x, y, 255);
            SetPixelV(image, x, y, 255);
        }
    }
        
    for( y = 0; y < W; y++ )
    {
        for( x = 0; x < H; x++ )
        {
            x0 = (((double)y / W * 3.5) - 2.5) * zoom + offseth;
            y0 = (((double)x / H * 2.0) - 1.0) * zoom + offsetv;
            x1 = 0.0;
            y1 = 0.0;
            i = 0;
            while( (x1 * x1) + (y1 * y1) < 2 * 2 && i < max_iteration)
            {
                num = (x1 * x1) - (y1 * y1) + x0;
                y1 = (2 * x1 * y1) + y0;
                x1 = num;
                i = i + 1;
            }
            color = i % MAX_COLOR;
            
            R = (unsigned char)palette[color][0];
            G = (unsigned char)palette[color][1];
            B = (unsigned char)palette[color][2];
            
            SetPixelR(image, x, y, R);
            SetPixelG(image, x, y, G);
            SetPixelB(image, x, y, B);
        }
    }

    return image;
}
