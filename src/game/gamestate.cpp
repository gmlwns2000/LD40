#include "gamestate.h"

ldjam::GameStateManager::GameStateManager(EngineRuntime & rt)
    :   m_rt(rt)
{
}

void ldjam::GameStateManager::Init()
{
    m_running = true;
}

void ldjam::GameStateManager::Cleanup()
{
    while (!m_states.empty())
    {
        m_states.back()->Cleanup(m_rt);
        m_states.pop_back();
    }
}

void ldjam::GameStateManager::ChangeState(IGameState * state)
{
    if (!m_states.empty())
    {
        m_states.back()->Cleanup(m_rt);
        m_states.pop_back();
    }

    m_states.push_back(state);
    m_states.back()->Init(m_rt);
}

void ldjam::GameStateManager::PushState(IGameState * state)
{
    if (!m_states.empty())
    {
        m_states.back()->Pause();
    }

    m_states.push_back(state);
    m_states.back()->Init(m_rt);
}

void ldjam::GameStateManager::PopState()
{
    if (!m_states.empty())
    {
        m_states.back()->Cleanup(m_rt);
        m_states.pop_back();
    }

    if (!m_states.empty())
    {
        m_states.back()->Resume();
    }
}

void ldjam::GameStateManager::HandleEvents(EngineRuntime& rt)
{
    m_states.back()->HandleEvents(this, rt);
}

void ldjam::GameStateManager::Update(EngineRuntime& rt, float delta)
{
    m_states.back()->Update(this, rt, delta);
}

void ldjam::GameStateManager::Render(EngineRuntime& rt)
{
    m_states.back()->Render(this, rt);
}
