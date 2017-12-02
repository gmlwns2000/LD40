#include "sprite.h"

void ldjam::RenderSprite(EngineRuntime& rt, const Sprite& sprite, SDL_RendererFlip flipMode)
{
    SDL_Rect clipQuad = 
    {
        0, 0,
        sprite.size.x, sprite.size.y
    };

    SDL_Rect renderQuad =
    {
        sprite.position.x, sprite.position.y,
        sprite.size.x, sprite.size.y
    };

    SDL_RenderCopyEx(rt.renderer, sprite.texture, &clipQuad, &renderQuad, sprite.rotation, nullptr, flipMode);
}
