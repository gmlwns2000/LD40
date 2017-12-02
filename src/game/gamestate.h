#pragma once
#include <vector>

#include "engine.h"

namespace ldjam
{
    class IGameState;

    class GameStateManager
    {
    public:
        GameStateManager(EngineRuntime& rt);

        void Init();
        void Cleanup();

        void ChangeState(IGameState* state);
        void PushState(IGameState* state);
        void PopState();

        void HandleEvents(EngineRuntime& rt);
        void Update(EngineRuntime& rt, float delta);
        void Render(EngineRuntime& rt);

        bool Running() { return m_running; }
        void Quit() { m_running = false; }
    
    private:
        std::vector<IGameState*> m_states;
        bool m_running = false;

        EngineRuntime& m_rt;
    };

    class IGameState
    {
    public:
        virtual void Init(EngineRuntime& rt) = 0;
        virtual void Cleanup(EngineRuntime& rt) = 0;

        virtual void Pause() = 0;
        virtual void Resume() = 0;

        virtual void HandleEvents(GameStateManager* game, EngineRuntime& rt) = 0;
        virtual void Update(GameStateManager* game, EngineRuntime& rt, float delta) = 0;
        virtual void Render(GameStateManager* game, EngineRuntime& rt) = 0;

        void ChangeState(GameStateManager* game, IGameState* state)
        {
            game->ChangeState(state);
        }
    };
}