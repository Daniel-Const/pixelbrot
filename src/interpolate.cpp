#include <math.h>
#include <algorithm>

#include "pixel.h"
#include "interpolate.h"

void downSample(Pixel *pixels, Pixel *original, int downWidth, int downHeight, int upWidth, int upHeight)
{
    // Fill the pixel array by downsampling the original larger pixel array

    float scaleX = (float)downWidth / (float)upWidth;
    float scaleY = (float)downHeight / (float)upHeight;

    for (int py = 0; py < downHeight; ++py)
    {
        for (int px = 0; px < downWidth; ++px)
        {
            int xNearest = std::floor((float)px / (float)scaleX);
            int yNearest = std::floor((float)py / (float)scaleY);

            Pixel pixel = original[upWidth * yNearest + xNearest];
            pixels[downWidth * py + px] = pixel;
        }
    }
}

void upSample(Pixel *pixels, Pixel *original, int upWidth, int upHeight, int downWidth, int downHeight)
{
    // Fill the pixels array by upsampling the original smaller pixels array

    float scaleX = (float)upWidth / (float)downWidth;
    float scaleY = (float)upHeight / (float)downHeight;

    for (int py = 0; py < upHeight; py++)
    {
        for (int px = 0; px < upWidth; px++)
        {
            int xNearest = std::floor((float)px / (float)scaleX);
            int yNearest = std::floor((float)py / (float)scaleY);

            Pixel pixel = original[downWidth * yNearest + xNearest];
            pixels[upWidth * py + px] = pixel;
        }
    }
}