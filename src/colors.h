struct Palette{
    const char *name;
    int (*colors)[3];
};

int standardPalette[16][3] = {
    {66, 30, 15},
    {25, 7, 26},
    {9, 1, 47},
    {4, 4, 73},
    {0, 7, 100},
    {12, 44, 138},
    {24, 82, 177},
    {57, 125, 209},
    {134, 181, 229},
    {211, 236, 248},
    {241, 233, 191},
    {248, 201, 95},
    {255, 170, 0},
    {204, 128, 0},
    {153, 87, 0},
    {106, 52, 3},
};

int otherPalette[16][3] = {
    {70, 22, 10},
    {25, 7, 26},
    {40, 200, 2},
    {120, 4, 73},
    {200, 7, 90},
    {12, 44, 50},
    {24, 82, 100},
    {57, 125, 0},
    {100, 200, 0},
    {130, 236, 100},
    {200, 233, 60},
    {160, 201, 95},
    {130, 100, 0},
    {100, 60, 40},
    {100, 87, 80},
    {106, 52, 100},
};

int black[3] = {0, 0, 0};





