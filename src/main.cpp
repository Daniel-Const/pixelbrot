#include <iostream>
#include <SDL2/SDL.h>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1200

const float scaleX[2] = {-2.00, 0.47};
const float scaleY[2] = {-1.12, 1.12};

int palette[16][3] = {
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

int black[3] = {0, 0, 0};

int *getColor(int iteration, int maxIteration)
{
    int i = iteration % 16;
    if (iteration > 0 && iteration < maxIteration)
    {
        return palette[i];
    }
    return black;
}

float mapToRange(float value, int oldMin, int oldMax, float newMin, float newMax)
{
    return ((value - oldMin) / (oldMax - oldMin)) * (newMax - newMin) + newMin;
}

/**
 * @brief Perform the mandelbrot plotting
 *
 * Algorithm from:
 * https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set
 *
 * @param renderer
 */
void mandelbrot(SDL_Renderer *renderer)
{
    const int maxIteration = 100;
    for (int px = 0; px < WINDOW_WIDTH; ++px)
    {
        for (int py = 0; py < WINDOW_HEIGHT; ++py)
        {
            float x0 = mapToRange(px, 0, WINDOW_WIDTH, scaleX[0], scaleX[1]);
            float y0 = mapToRange(py, 0, WINDOW_HEIGHT, scaleY[0], scaleY[1]);
            float x = 0;
            float y = 0;
            int iteration = 0;
            while (x * x + y * y <= 2 * 2 && iteration < maxIteration)
            {
                float xTemp = x * x - y * y + x0;
                y = 2 * x * y + y0;
                x = xTemp;
                iteration++;
            }

            int *color = getColor(iteration, maxIteration);
            SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);
            SDL_RenderDrawPoint(renderer, px, py);
        }
    }
}

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Pixelbrot", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_Event event;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    mandelbrot(renderer);
    SDL_RenderPresent(renderer);
    while (1)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}