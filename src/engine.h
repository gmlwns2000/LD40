#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <glm/glm.hpp>

#include "xml/windowloader.h"
#include "util/types.h"
#include "graphics/texturemanager.h"

constexpr auto ldConfigFilename     = "ld40-config.xml";
constexpr auto ldLogFilename        = "ld40-log.txt";
constexpr auto ldLogDebugFilename   = "ld40-log-debug.txt";

constexpr auto ldFramerateFontSize  = 50;

constexpr auto ldDisplayBaseWidth   = 800;
constexpr auto ldDisplayBaseHeight  = 600;

#define LD_TEXTURE_DIR                "assets/textures/"
#define LD_FONT_DIR                   "assets/fonts/"

namespace ldjam
{x
    struct EngineRuntime
    {
        SDL_Window   *window                = nullptr;
        SDL_Renderer *renderer              = nullptr;
                                            
        const uint8  *keyboardState         = nullptr;
        glm::ivec2    mousePosition         = { 0, 0 };

        ldjam::WindowConfig windowConfig    = {};
        TTF_Font *debugFont                 = nullptr;

        TextureManager textureManager       = TextureManager(nullptr);

        bool showDebugOverlay
#ifdef SHIPPING
                                            = false;
#else
                                            = true;
#endif
    };

    inline bool KeyDown(EngineRuntime& runtime, SDL_Scancode key)
    {
        return runtime.keyboardState[key] == 0x01;
    }
}