#pragma once

#include <memory>

namespace Engine {
class TimeState {
   public:
    TimeState(uint64_t startCounter);

    void NextTimestep(uint64_t currentCounter, uint64_t counterFrequency);

    inline float GetTimeScale() const { return m_TimeScale; }
    void SetTimeScale(float value) {
        if (value < 0) value = 0;

        m_TimeScale = value;
    }

    inline float GetUnscaledTime() const { return m_UnscaledTime; }
    inline float GetTime() const { return m_Time; }

    inline float GetUnscaledDeltaTime() const { return m_UnscaledDeltaTime; }
    inline float GetDeltaTime() const { return m_DeltaTime; }

   private:
    uint64_t m_Counter;

    float m_TimeScale;

    float m_UnscaledTime;
    float m_Time;

    float m_UnscaledDeltaTime;
    float m_DeltaTime;
};
}  // namespace Engine