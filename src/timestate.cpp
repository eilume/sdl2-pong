#include "timestate.h"

#include <iostream>

namespace Engine {
TimeState::TimeState(uint64_t startCounter)
    : m_Counter(startCounter),
      m_TimeScale(1.0f),
      m_UnscaledTime(0.0f),
      m_Time(0.0f),
      m_UnscaledDeltaTime(0.0f),
      m_DeltaTime(0.0f) {}

void TimeState::NextTimestep(uint64_t currentCounter,
                             uint64_t counterFrequency) {
    float unscaledDelta =
        (float)((currentCounter - m_Counter) / (double)counterFrequency);

    std::cout << unscaledDelta << std::endl;

    m_UnscaledDeltaTime = unscaledDelta;
    m_DeltaTime = unscaledDelta * m_TimeScale;

    m_UnscaledTime += unscaledDelta;
    m_Time += m_DeltaTime;

    m_Counter = currentCounter;
}
}  // namespace Engine