#include "pixel.h"

void downSample(Pixel *pixels, Pixel *original, int downWidth, int downHeight, int upWidth, int upHeight);
void upSample(Pixel *pixels, Pixel *original, int upWidth, int upHeight, int downWidth, int downHeight);
