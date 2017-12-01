#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "engine.h"
#include "util/log.h"
#include "util/timer.h"
#include "xml/windowloader.h"

using namespace ldjam;

void LoadConfig();
void SaveConfig();

void LoadFonts();
void UnloadFonts();

void RenderDebugString(EngineRuntime& runtime, const char* str);

int main(int argc, char** argv)
{
#ifdef SHIPPING
    log::SetLevel(LogLevel::Info);
    log::SetFile(ldLogFilename);
    log::SetQuiet(true);
#else
    log::SetLevel(LogLevel::Debug);
    log::SetFile(ldLogDebugFilename);
    log::SetQuiet(false);
#endif

    // -- SDL2 init
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        AbortProcess("Failed to initialize SDL2! Make sure that your drivers are updated.");
    if (TTF_Init() < 0)
        AbortProcess("Failed to initialize SDL_TTF! It's probably an issue with freetype");

    LoadConfig();
    LoadFonts();

    EngineRuntime runtime;

    runtime.window = SDL_CreateWindow(
        "LudumDare #40",
        ldWindowConfig.x, ldWindowConfig.y,
        ldWindowConfig.width, ldWindowConfig.height,
        SDL_WINDOW_SHOWN);

    if (!runtime.window)
        AbortProcess("Failed to initialize SDL2 window!");

    runtime.renderer = SDL_CreateRenderer(runtime.window, -1, SDL_RENDERER_ACCELERATED);
    if (!runtime.renderer)
        AbortProcess("Failed to initialize SDL2 renderer!");

    SDL_SetRenderDrawColor(runtime.renderer, 0x00, 0x00, 0x00, 0xff);

    SDL_Event sdlEvent;
    bool running = true;

    Timer fpsTimer;
    Timer fpsCapTimer;
    int frameCount = 0;

    fpsTimer.Start();
    while (running)
    {
        fpsCapTimer.Start();

        // Get framerate
        float avgFPS = frameCount / (fpsTimer.GetTicks() / 1000.f);
        if (avgFPS > 2000000)
        {
            avgFPS = 0;
        }

        // Input
        runtime.keyboardState = SDL_GetKeyboardState(nullptr);
        SDL_GetMouseState(&runtime.mouseState[0], &runtime.mouseState[1]);

        while (SDL_PollEvent(&sdlEvent) != 0)
        {
            if (sdlEvent.type == SDL_QUIT) running = false;
        }

        // Rendering
        SDL_RenderClear(runtime.renderer);

        // Display framerate
        if (ldShowFPS)
        {
            SDL_Surface* surface = TTF_RenderText_Shaded(ldDebugFont,
                std::string("FPS: ").append(std::to_string(avgFPS)).c_str(),
                { 0, 255, 0, 255 },
                { 0, 0, 0, 255 });

            SDL_Texture* texture = SDL_CreateTextureFromSurface(runtime.renderer, surface);
            if (texture)
            {
                SDL_Rect srcRect = { 0, 0, surface->w, surface->h };
                SDL_RenderCopy(runtime.renderer, texture, &srcRect, &srcRect);
            }

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        SDL_RenderPresent(runtime.renderer);

        frameCount++;
        int frameTicks = fpsCapTimer.GetTicks();
        if (frameTicks < ldFramerateTicks)
        {
            // Sleep
            SDL_Delay(ldFramerateTicks - frameTicks);
        }
    }

    SaveConfig();

    // Dipose everything
    LD_LOG_DEBUG("Disposing SDL resources");
    SDL_DestroyRenderer(runtime.renderer);
    SDL_DestroyWindow(runtime.window);

    TTF_Quit();
    SDL_Quit();
    return 0;
}

void LoadConfig()
{
    LD_LOG_INFO("Loading config");
    ldWindowConfig = config::LoadWindow(ldConfigFilename);
}

void SaveConfig()
{
    LD_LOG_DEBUG("Saving current config to %s", ldConfigFilename);
    FILE* cfgFile = fopen(ldConfigFilename, "w");

    config::WriteWindow(ldWindowConfig, cfgFile);

    fclose(cfgFile);
}

void LoadFonts()
{
    LD_LOG_INFO("Loading fonts");

    ldDebugFont = TTF_OpenFont("assets/fonts/Debug.ttf", ldFramerateFontSize);
    if (!ldDebugFont) LD_LOG_ERROR("Failed to load font assets/fonts/Debug.ttf !");
}

void UnloadFonts()
{
    LD_LOG_INFO("Unloading fonts");
    if (ldDebugFont) TTF_CloseFont(ldDebugFont);
}

void RenderDebugString(EngineRuntime& runtime, const char * str)
{
    SDL_Surface* surface = TTF_RenderText_Shaded(ldDebugFont,
        str,
        { 0, 255, 0, 255 },
        { 0, 0, 0, 255 });

    SDL_Texture* texture = SDL_CreateTextureFromSurface(runtime.renderer, surface);
    if (texture)
    {
        SDL_Rect srcRect = { 0, 0, surface->w, surface->h };
        SDL_RenderCopy(runtime.renderer, texture, &srcRect, &srcRect);
    }

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
