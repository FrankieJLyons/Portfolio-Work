#include <cstdlib>
#include <ctime>

#include <raylib.h>

#include "vectorMath.h"

#define MAX_PARTICLES 100

class Particle
{
private:
    VectorMath & vm = VectorMath::getInstance();

public:
    Vector2 position;
    Vector2 velocity;
    Color color;

    Particle()
    {
        position = (Vector2){ vm.GetRandomValue(0, 800), vm.GetRandomValue(0, 450) };
        velocity = (Vector2){ vm.GetRandomValue(-100, 100) / 100.0f, vm.GetRandomValue(-100, 100) / 100.0f };
        color = (Color){ (unsigned char)(GetRandomValue(0, 255) * 255.0f), 
                 (unsigned char)(GetRandomValue(0, 255) * 255.0f), 
                 (unsigned char)(GetRandomValue(0, 255) * 255.0f), 
                 255 };
    }

    void Update()
    {
        position.x += velocity.x;
        position.y += velocity.y;
    }

    void Draw()
    {
        DrawPixel((int)position.x, (int)position.y, color);
    }
};