#pragma once
#include "gamestate.h"
#include "graphics/sprite.h"

namespace ldjam
{
    class MenuState : public IGameState
    {
    public:
        virtual void Init(EngineRuntime& rt);
        virtual void Cleanup(EngineRuntime& rt);

        virtual void Pause();
        virtual void Resume();

        virtual void HandleEvents(GameStateManager* game, EngineRuntime& rt);
        virtual void Update(GameStateManager* game, EngineRuntime& rt, float delta);
        virtual void Render(GameStateManager* game, EngineRuntime& rt);

        static MenuState* Instance() { return &s_instance; }

    private:
        static MenuState s_instance;
    
    private:
        Sprite testSprite;
    };
}