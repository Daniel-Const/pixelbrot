#include <SDL2/SDL.h>
#include <SDL_opengl.h>

#include "interpolate.h"
#include "pixel.h"
#include "colors.h"
#include "image.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

int WINDOW_HEIGHT = 800;
int WINDOW_WIDTH = 1200;

int DOWNSCALE_HEIGHT = 300;
int DOWNSCALE_WIDTH = 500;

const float scaleX[2] = {-2.00, 0.47};
const float scaleY[2] = {-1.12, 1.12};

int (*palette)[3] = standardPalette;

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
void mandelbrot(Pixel *pixels)
{
    const int maxIteration = 100;
    for (int py = 0; py < WINDOW_HEIGHT; ++py)
    {
        for (int px = 0; px < WINDOW_WIDTH; ++px)
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

            Pixel pixel;
            pixel.color = getColor(iteration, maxIteration);
            pixels[WINDOW_WIDTH * py + px] = pixel;
        }
    }
}

void createMandelbrotTexture(SDL_Renderer *renderer, SDL_Texture *texture) {

    SDL_RenderClear(renderer);
    SDL_SetRenderTarget( renderer, texture );

    Pixel *pixels = new Pixel[WINDOW_HEIGHT * WINDOW_WIDTH];
    Pixel *downPixels = new Pixel[DOWNSCALE_WIDTH * DOWNSCALE_HEIGHT];

    mandelbrot(pixels);
    downSample(downPixels, pixels, DOWNSCALE_WIDTH, DOWNSCALE_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT);
    upSample(pixels, downPixels, WINDOW_WIDTH, WINDOW_HEIGHT, DOWNSCALE_WIDTH, DOWNSCALE_HEIGHT);
    
    // Draw mandelbrot pixels to the texture
    for (int py = 0; py < WINDOW_HEIGHT; py++)
    {
        for (int px = 0; px < WINDOW_WIDTH; px++)
        {
            int *color = pixels[WINDOW_WIDTH * py + px].color;
            SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);
            SDL_RenderDrawPoint(renderer, px, py);
        }
    }

    SDL_SetRenderTarget( renderer, NULL );

    free(pixels);
    free(downPixels);
}

int main(int argc, char *argv[])
{
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Pixelbrot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1);
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    SDL_Texture* texture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT);
    createMandelbrotTexture(renderer, texture);
    bool loop = true;
    static int selectedColorPalette = 0;
    int menuWidth = 280;
    int menuHeight = 80;

    while (loop)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);

            switch (event.type)
            {
            case SDL_QUIT:
                loop = false;
                break;
            }
        }

        // Imgui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
        ImGui::Begin("Color palettes");
        ImGui::SetWindowSize(ImVec2(menuWidth, menuHeight));
        ImGui::SetWindowPos(ImVec2(0, (float)(WINDOW_HEIGHT - menuHeight)));
       {
            const char* items[] = { "Standard", "Funky"};
            ImGui::Combo("select", &selectedColorPalette, items, IM_ARRAYSIZE(items));
	    // ImGui::Button("screenshot");


        }
        ImGui::End();
        ImGui::EndFrame();

	// TODO: Create tiny litle sample textures / images to display preview
	// 	 Add Screenshot button

        if (selectedColorPalette == 0 && palette != standardPalette) {
            palette = standardPalette;
            createMandelbrotTexture(renderer, texture);
        }

        if (selectedColorPalette == 1 && palette != otherPalette) {
            palette = otherPalette;
            createMandelbrotTexture(renderer, texture);
        }


        // Render: SDL
        glClearColor(0, 0, 0, 255);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_RenderCopy (renderer, texture, 0, NULL);

        // Render: ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }


    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyTexture( texture );
    texture = NULL;


    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
