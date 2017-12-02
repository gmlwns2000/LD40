#pragma once
#include <SDL.h>
#include <glm/vec2.hpp>

#include "util/timer.h"
#include "engine.h"

namespace ldjam
{
    class Sprite
    {
    public:
        glm::ivec2 position;
        glm::ivec2 size;
        double rotation = 0.0f;

    public:
        Sprite() 
            : 
            m_texture(nullptr),
            m_baseSize(0, 0)
        {}

        void SetTexture(SDL_Texture *texture);
        SDL_Texture* GetTexture();

        glm::ivec2 GetBaseSize() const { return m_baseSize; }
    private:
        SDL_Texture *m_texture;
        glm::ivec2 m_baseSize;
    };

    void RenderSprite(EngineRuntime& rt, Sprite& sprite, SDL_RendererFlip flipMode = SDL_FLIP_NONE);
}