#pragma once
#include "util/types.h"

namespace ldjam
{
    class Timer
    {
    public:
        Timer();

        void Start();
        void Stop();
        void Pause();
        void UnPause();

        void Reset();

        uint32 GetTicks();

        bool Started();
        bool Paused();

    private:
        uint32 m_startTicks;
        uint32 m_pausedTicks;

        bool m_paused;
        bool m_started;
    };
}