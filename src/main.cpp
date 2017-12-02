#include <string>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "engine.h"
#include "util/log.h"
#include "util/timer.h"
#include "xml/windowloader.h"
#include "game/gamestate.h"
#include "game/menu_state.h"

using namespace ldjam;

void LoadConfig(EngineRuntime& rt);
void SaveConfig(EngineRuntime& rt);

void LoadFonts(EngineRuntime& rt);
void UnloadFonts(EngineRuntime& rt);

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
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)         AbortProcess("Failed to initialize SDL2! Make sure that your drivers are updated.");
    if (TTF_Init() < 0)                             AbortProcess("Failed to initialize SDL_TTF! It's probably an issue with freetype");

    EngineRuntime rt;

    LoadConfig(rt);
    LoadFonts(rt);

    rt.window = SDL_CreateWindow(
        "LudumDare #40",
        rt.windowConfig.x, rt.windowConfig.y,
        rt.windowConfig.width, rt.windowConfig.height,
        SDL_WINDOW_SHOWN);

    if (!rt.window)
        AbortProcess("Failed to initialize SDL2 window!");

    rt.renderer = SDL_CreateRenderer(rt.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!rt.renderer)
        AbortProcess("Failed to initialize SDL2 renderer!");

    rt.textureManager = TextureManager(rt.renderer);

    // Set clear color
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_SetRenderDrawColor(rt.renderer, 0, 0, 0, 255);

    GameStateManager game(rt);
    game.Init();
    game.ChangeState(MenuState::Instance());

    SDL_Event sdlEvent;
    while (game.Running())
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

        // Update
        // ------------------------------------------------ //
        game.Update(rt, (float)elapsed / 1000.0f);

        // Event handling
        // ------------------------------------------------ //
        rt.keyboardState = SDL_GetKeyboardState(nullptr);
        
        SDL_GetMouseState(&rt.mousePosition.x , &rt.mousePosition.y);

        rt.mousePosition.x /= ((double)rt.windowConfig.width / ldDisplayBaseWidth);;
        rt.mousePosition.y /= ((double)rt.windowConfig.height / ldDisplayBaseHeight);;


        while (SDL_PollEvent(&sdlEvent) != 0)
        {
            if (sdlEvent.type == SDL_QUIT) game.Quit();
        }

        SDL_GetWindowPosition(rt.window, &rt.windowConfig.x, &rt.windowConfig.y);
        
#ifndef SHIPPING
        if (rt.keyboardState[SDL_SCANCODE_SLASH] == 1)
            rt.showDebugOverlay = true;
        else
            rt.showDebugOverlay = false;
#endif
        game.HandleEvents(rt);

        // Rendering
        // ------------------------------------------------ //
        SDL_RenderClear(rt.renderer);

        game.Render(rt);

        if (rt.showDebugOverlay)
            RenderDebugString(rt, std::string("FPS: ").append(std::to_string(fps)).c_str(), 0, 0);
        
        SDL_RenderPresent(rt.renderer);
    }

    SaveConfig(rt);

    // Dipose everything
    game.Cleanup();

    LD_LOG_DEBUG("Disposing SDL resources");
    SDL_DestroyRenderer(rt.renderer);
    SDL_DestroyWindow(rt.window);

    TTF_Quit();
    SDL_Quit();
    return 0;
}

void LoadConfig(EngineRuntime& rt)
{
    LD_LOG_INFO("Loading config");
    rt.windowConfig = config::LoadWindow(ldConfigFilename);
}

void SaveConfig(EngineRuntime& rt)
{
    LD_LOG_DEBUG("Saving current config to %s", ldConfigFilename);
    FILE* cfgFile = fopen(ldConfigFilename, "w");

    config::WriteWindow(rt.windowConfig, cfgFile);

    fclose(cfgFile);
}

void LoadFonts(EngineRuntime& rt)
{
    LD_LOG_INFO("Loading fonts");

    rt.debugFont = TTF_OpenFont(LD_FONT_DIR "Debug.ttf", ldFramerateFontSize);
    if (!rt.debugFont) LD_LOG_ERROR("Failed to load font assets/fonts/Debug.ttf !");
}

void UnloadFonts(EngineRuntime& rt)
{
    LD_LOG_INFO("Unloading fonts");
    if (rt.debugFont) TTF_CloseFont(rt.debugFont);
}

void RenderDebugString(EngineRuntime& rt, const char *str, int32 x, int32 y)
{
    SDL_Surface* surface = TTF_RenderText_Shaded(rt.debugFont,
        str,
        { 0, 255, 0, 255 },
        { 0, 0, 0, 0 });

    SDL_Texture* texture = SDL_CreateTextureFromSurface(rt.renderer, surface);
    if (texture)
    {
        SDL_Rect srcRect = { 0, 0, surface->w, surface->h };
        SDL_Rect dstRect = { x, y, surface->w, surface->h };
        SDL_RenderCopy(rt.renderer, texture, &srcRect, &dstRect);
    }

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
