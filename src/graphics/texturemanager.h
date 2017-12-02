#pragma once

#include <unordered_map>
#include <map>
#include <SDL.h>

namespace ldjam
{
    class TextureManager
    {
    public:
        TextureManager(SDL_Renderer *renderer) 
            : m_renderer(renderer) 
        {}

        SDL_Texture* Load(const char* ID, const char* filename);
        SDL_Texture* Load(const char* ID);
        
        void UnLoad(const char* ID);

        SDL_Texture* operator[] (const char* ID)
        {
            if (m_textureMap.find(ID) != m_textureMap.end())
                return m_textureMap.at(ID);
            else
                return nullptr;
        }

    private:
        SDL_Renderer *m_renderer;
        std::map<const char*, SDL_Texture*> m_textureMap;
    };
}