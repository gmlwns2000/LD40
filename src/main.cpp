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

void RenderDebugString(EngineRuntime& runtime, const char* str, int32 x, int32 y);

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

    runtime.renderer = SDL_CreateRenderer(runtime.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!runtime.renderer)
        AbortProcess("Failed to initialize SDL2 renderer!");

    SDL_SetRenderDrawColor(runtime.renderer, 139, 0, 139, 255);

    SDL_Event sdlEvent;
    bool running = true;


    while (running)
    {
        // Compute FPS
        // ------------------------------------------------ //

        constexpr auto fpsAvgSampleCount = 10;

        static uint32 nowTime = 0;
        static uint32 lastTime = 0;
        static uint32 cycleCount = 0;

        lastTime = nowTime;
        nowTime = SDL_GetTicks();
        uint32 elapsed = nowTime - lastTime;

        static uint32 frameTimes[fpsAvgSampleCount];
        frameTimes[cycleCount % fpsAvgSampleCount] = elapsed;

        uint32 fpsTicks = 0;
        for (uint32 i = 0; i < fpsAvgSampleCount; i++)
            fpsTicks += frameTimes[i];

        float fps = 1000.0f / (fpsTicks / fpsAvgSampleCount);

        cycleCount++;


        // Event handling
        // ------------------------------------------------ //

        runtime.keyboardState = SDL_GetKeyboardState(nullptr);
        SDL_GetMouseState(&runtime.mouseState[0], &runtime.mouseState[1]);

        while (SDL_PollEvent(&sdlEvent) != 0)
        {
            if (sdlEvent.type == SDL_QUIT) running = false;
        }

#ifndef SHIPPING
        if (runtime.keyboardState[SDL_SCANCODE_SLASH] == 1)
            ldShowDebugOverlay = true;
        else
            ldShowDebugOverlay = false;
#endif

        // Rendering
        // ------------------------------------------------ //

        SDL_RenderClear(runtime.renderer);

        if (ldShowDebugOverlay)
            RenderDebugString(runtime, std::string("FPS: ").append(std::to_string(fps)).c_str(), 0, 0);

        SDL_RenderPresent(runtime.renderer);
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

void RenderDebugString(EngineRuntime& runtime, const char *str, int32 x, int32 y)
{
    SDL_Surface* surface = TTF_RenderText_Shaded(ldDebugFont,
        str,
        { 0, 255, 0, 255 },
        { 0, 0, 0, 0 });

    SDL_Texture* texture = SDL_CreateTextureFromSurface(runtime.renderer, surface);
    if (texture)
    {
        SDL_Rect srcRect = { 0, 0, surface->w, surface->h };
        SDL_Rect dstRect = { x, y, surface->w, surface->h };
        SDL_RenderCopy(runtime.renderer, texture, &srcRect, &dstRect);
    }

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
