#include "collision.h"

BVec2 Collision::Bounce(Entity* object, const SDL_Rect& obstacle,
                        bool inverseObstacle) {
    BVec2 output = BVec2();
    if (!inverseObstacle) {
        if (object->pos.x < obstacle.x + obstacle.w &&
            object->pos.x + object->rect.w > obstacle.x &&
            object->pos.y < obstacle.y + obstacle.h &&
            object->pos.y + object->rect.h > obstacle.y) {
            // TODO: correct position here!
            output.x = true;
            output.y = true;
        }
    } else {
        if (object->pos.x < obstacle.x) {
            object->pos.x = obstacle.x - object->pos.x;
            output.x = true;
        } else if (object->pos.x + object->rect.w > obstacle.x + obstacle.w) {
            object->pos.x =
                obstacle.x + obstacle.w - object->rect.w -
                (object->pos.x + object->rect.w - (obstacle.x + obstacle.w));
            output.x = true;
        }

        if (object->pos.y < obstacle.y) {
            object->pos.y = obstacle.y - object->pos.y;
            output.y = true;
        } else if (object->pos.y + object->rect.h > obstacle.y + obstacle.h) {
            object->pos.y =
                obstacle.y + obstacle.h - object->rect.h -
                (object->pos.y + object->rect.h - (obstacle.y + obstacle.h));
            output.y = true;
        }
    }

    return output;
}