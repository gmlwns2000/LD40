#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "xml/windowloader.h"
#include "util/types.h"

constexpr auto ldConfigFilename     = "ld40-config.xml";
constexpr auto ldLogFilename        = "ld40-log.txt";
constexpr auto ldLogDebugFilename   = "ld40-log-debug.txt";

constexpr auto ldFramerateFontSize  = 21;

ldjam::WindowConfig ldWindowConfig  = {};
TTF_Font *ldDebugFont               = nullptr;


#ifdef SHIPPING
bool ldShowFPS                      = false;
#else
bool ldShowDebugOverlay                      = true;
#endif

namespace ldjam
{
    struct EngineRuntime
    {
        SDL_Window   *window        = nullptr;
        SDL_Renderer *renderer      = nullptr;

        const uint8  *keyboardState = nullptr;
        int           mouseState[2] = { 0, 0 };
    };
}