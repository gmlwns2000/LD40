#include "sprite.h"

void ldjam::RenderSprite(EngineRuntime& rt, Sprite& sprite, SDL_RendererFlip flipMode)
{
    // Aspect ratio - always 800:600
    double positionDt = ((double)rt.windowConfig.width / ldDisplayBaseWidth);

    SDL_Rect clipQuad =
    {
        0, 0,
        sprite.GetBaseSize().x, sprite.GetBaseSize().y
    };

    SDL_Rect renderQuad =
    {
        sprite.position.x * positionDt, sprite.position.y * positionDt,
        sprite.size.x * positionDt, sprite.size.y * positionDt
    };

    SDL_RenderCopyEx(rt.renderer, sprite.GetTexture(), &clipQuad, &renderQuad, sprite.rotation, nullptr, flipMode);
}

void ldjam::Sprite::SetTexture(SDL_Texture * texture)
{
    m_texture = texture;

    SDL_QueryTexture(texture, nullptr, nullptr, &m_baseSize.x, &m_baseSize.y);
}

SDL_Texture* ldjam::Sprite::GetTexture()
{
    return m_texture;
}

