#include "collision.h"

BVec2 Collision::Collide(Entity* object, const SDL_Rect& obstacle,
                         bool inverseObstacle) {
    BVec2 output = BVec2();
    if (!inverseObstacle) {
        if (object->pos.x < obstacle.x + obstacle.w &&
            object->pos.x + object->rect.w > obstacle.x &&
            object->pos.y < obstacle.y + obstacle.h &&
            object->pos.y + object->rect.h > obstacle.y) {
            // TODO: adjust position here!
            output.x = true;
            output.y = true;
        }
    } else {
        if (object->pos.x < obstacle.x) {
            object->pos.x = obstacle.x;
            output.x = true;
        } else if (object->pos.x + object->rect.w > obstacle.x + obstacle.w) {
            object->pos.x = obstacle.x + obstacle.w - object->rect.w;
            output.x = true;
        }

        if (object->pos.y < obstacle.y) {
            object->pos.y = obstacle.y;
            output.y = true;
        } else if (object->pos.y + object->rect.h > obstacle.y + obstacle.h) {
            object->pos.y = obstacle.y + obstacle.h - object->rect.h;
            output.y = true;
        }
    }
    return output;
}

BVec2 Collision::Bounce(Entity* object, const SDL_Rect& obstacle,
                        bool inverseObstacle) {
    BVec2 output = BVec2();
    if (!inverseObstacle) {
        // Normal obstacle
        if (object->pos.x < obstacle.x + obstacle.w &&
            object->pos.x + object->rect.w > obstacle.x &&
            object->pos.y < obstacle.y + obstacle.h &&
            object->pos.y + object->rect.h > obstacle.y) {
            // TODO: this is _VERY_ hacked together
            if (object->pos.x + object->rect.w > obstacle.x &&
                object->pos.x + object->rect.w - object->vel.x < obstacle.x) {
                object->pos.x = obstacle.x - object->rect.w -
                                (object->pos.x + object->rect.w - obstacle.x);
                object->vel.x *= -1.0f;
                output.x = true;
            } else if (object->pos.x < obstacle.x + obstacle.w &&
                       object->pos.x - object->vel.x >
                           obstacle.x + obstacle.w) {
                object->pos.x = obstacle.x + obstacle.w +
                                (obstacle.x + obstacle.w - object->pos.x);
                object->vel.x *= -1.0f;
                output.x = true;
            }
        }
    } else {
        // Inverted obstacle
        if (object->pos.x < obstacle.x) {
            object->pos.x = obstacle.x - object->pos.x;
            object->vel.x *= -1.0f;
            output.x = true;
        } else if (object->pos.x + object->rect.w > obstacle.x + obstacle.w) {
            object->pos.x =
                obstacle.x + obstacle.w - object->rect.w -
                (object->pos.x + object->rect.w - (obstacle.x + obstacle.w));
            object->vel.x *= -1.0f;
            output.x = true;
        }

        if (object->pos.y < obstacle.y) {
            object->pos.y = obstacle.y - object->pos.y;
            object->vel.y *= -1.0f;
            output.y = true;
        } else if (object->pos.y + object->rect.h > obstacle.y + obstacle.h) {
            object->pos.y =
                obstacle.y + obstacle.h - object->rect.h -
                (object->pos.y + object->rect.h - (obstacle.y + obstacle.h));
            object->vel.y *= -1.0f;
            output.y = true;
        }
    }

    return output;
}