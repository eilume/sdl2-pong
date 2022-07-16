#pragma once

#include "entity.h"

class Collision {
   public:
    static BVec2 Collide(Entity* object, const SDL_Rect& obstacle,
                         bool inverseObstacle = false);
    static BVec2 Bounce(Entity* object, const SDL_Rect& obstacle,
                        bool inverseObstacle = false);
};