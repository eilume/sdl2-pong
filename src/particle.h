#pragma once

#include <SDL.h>

#include <iostream>
#include <random>
#include <vector>

#include "entity.h"
#include "mathutils.h"
#include "timestate.h"

class ParticleSystem {
   public:
    ParticleSystem(int size, std::shared_ptr<Texture> tex, std::mt19937* gen)
        : m_IsActive(false), m_Size(size), m_Gen(gen) {
        m_ValueDist = std::uniform_real_distribution<float>(0.0f, 1.0f);
        m_Entities.reserve(m_Size);
        for (int i = 0; i < m_Size; i++) {
            m_Entities.emplace_back(tex);
        }
    }

    void Start(Vec2 startPos, float angleRange, float angleOffset) {
        m_StartPos = startPos;
        for (int i = 0; i < m_Size; i++) {
            m_Entities[i].pos = Vec2(startPos);
            m_Entities[i].vel =
                Vec2(1, 0).RotateByDeg(
                    MathUtils::CustomRange(m_ValueDist(*m_Gen), 0, angleRange) +
                    angleOffset) *
                MathUtils::CustomRange(m_ValueDist(*m_Gen), 50.0f, 150.0f);
            m_Entities[i].scale = 1.5f;
            m_Entities[i].angle = m_Entities[i].vel.GetAngleDeg();
        }

        m_IsActive = true;
    }
    void ForceStop() {}

    bool IsActive() const { return m_IsActive; }
    Vec2* GetStartPos() { return &m_StartPos; }

    void Update(Engine::TimeState& timeState) {
        if (m_IsActive) {
            for (int i = 0; i < m_Size; i++) {
                m_Entities[i].Update(timeState);
                m_Entities[i].vel = Vec2::Lerp(m_Entities[i].vel, Vec2(),
                                               timeState.GetDeltaTime() * 2.5f);
                m_Entities[i].scale = MathUtils::Lerp(
                    m_Entities[i].scale, 0.0f, timeState.GetDeltaTime() * 5.0f);
                m_Entities[i].UpdateRect();
            }

            if (m_Entities[0].scale <= 0.001f) m_IsActive = false;
        }
    }

    void Render(SDL_Renderer* renderer) {
        if (m_IsActive) {
            for (int i = 0; i < m_Size; i++) {
                m_Entities[i].Render(renderer);
            }
        }
    }

   private:
    bool m_IsActive;
    int m_Size;
    std::mt19937* m_Gen;
    std::uniform_real_distribution<float> m_ValueDist;
    Vec2 m_StartPos;
    std::vector<Entity> m_Entities;
};