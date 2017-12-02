#include <SDL_image.h>

#include "util/log.h"
#include "util/types.h"
#include "texturemanager.h"

#define SDL_STBIMAGE_IMPLEMENTATION
#include "graphics/SDL_stbimage.h"

SDL_Texture *ldjam::TextureManager::Load(const char *ID, const char *filename)
{
    auto existingTex = (*this)[ID];
    if ((*this)[ID] != nullptr)
    {
        LD_LOG_WARN("Attempting to register texture twice! (%s)", filename);
        return existingTex;
    }

    // Load texture
    LD_LOG_INFO("Loading texture @%s (%s)", ID, filename);

    SDL_Texture *texture = nullptr;
    SDL_Surface *surface = STBIMG_Load(filename);
    if (!surface)
    {
        LD_LOG_ERROR("Failed to load texture %s, from file %s", ID, filename);
        return nullptr;
    }

    texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_FreeSurface(surface);
    
    if (!texture)
    {
        LD_LOG_ERROR("Failed to create texture from file %s", filename);
        return nullptr;
    }

    m_textureMap.insert(std::make_pair(ID, texture));
    return texture;
}

SDL_Texture *ldjam::TextureManager::Load(const char *ID)
{
    return (*this)[ID];
}

void ldjam::TextureManager::UnLoad(const char *ID)
{
    auto existingTex = (*this)[ID];
    if ((*this)[ID] != nullptr)
    {
        existingTex = nullptr;
        LD_LOG_DEBUG("Unloaded texture @%s", ID);
        return;
    }

    LD_LOG_WARN("Attempting to unload texture @%s twice!", ID);
}
