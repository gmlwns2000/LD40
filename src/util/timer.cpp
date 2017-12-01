#include <SDL.h>
#include "timer.h"

ldjam::Timer::Timer()
    :
        m_startTicks(0),
        m_pausedTicks(0),
        m_paused(false),
        m_started(false)
{
}

void ldjam::Timer::Start()
{
    m_started = true;
    m_paused = false;
    m_startTicks = SDL_GetTicks();
    m_pausedTicks = 0;
}

void ldjam::Timer::Stop()
{
    m_started = false;
    m_paused = false;
    m_startTicks = 0;
    m_pausedTicks = 0;
}

void ldjam::Timer::Pause()
{
    if (m_started && !m_paused)
    {
        m_paused = true;
        m_pausedTicks = SDL_GetTicks() - m_startTicks;
        m_startTicks = 0;
    }
}

void ldjam::Timer::UnPause()
{
    if (m_started && m_paused)
    {
        m_paused = false;
        m_startTicks = SDL_GetTicks() - m_pausedTicks;
        m_pausedTicks = 0;
    }
}

uint32 ldjam::Timer::GetTicks()
{
    Uint32 time = 0;
    if (m_started)
    {
        if (m_paused)
            time = m_pausedTicks;
        else
            time = SDL_GetTicks() - m_startTicks;
    }
    return time;
}

bool ldjam::Timer::Started()
{
    return m_started;
}

bool ldjam::Timer::Paused()
{
    return m_paused && m_started;
}
