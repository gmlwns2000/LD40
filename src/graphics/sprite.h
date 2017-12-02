#pragma once
#include <SDL.h>
#include <glm/vec2.hpp>

#include "util/timer.h"
#include "engine.h"

namespace ldjam
{
    struct Sprite
    {
        SDL_Texture *texture;
        glm::ivec2 position;
        glm::ivec2 size;
        double rotation = 0.0f;
    };

    void RenderSprite(EngineRuntime& rt, const Sprite& sprite, SDL_RendererFlip flipMode = SDL_FLIP_NONE);
}