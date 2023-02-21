#include <vector>
#include <memory>

#include <raylib.h>

#include "vectorMath.h"

const int maxParticles = 64;
const float particleLifetime = 0.64f;
const float particleSpeed = 64.0f;
const float particleSpread = 12.8f;
const float spawnDiameter = 16.0f;

class ParticleSpawner {
private:
    VectorMath* vm;

    int spawnCount = 0;

public:
    Vector2 position;
    Color color;

    struct Particle {
        Vector2 position;
        Color color;
        float angle;
        float life;
    };

    std::vector<Particle> particles;

    ParticleSpawner(Vector2 initialPos, Color initialColor) : position(initialPos), color(initialColor) {
        vm = &VectorMath::getInstance();
    }

    ~ParticleSpawner() {
        particles.clear();
    }

    ParticleSpawner(const ParticleSpawner& other) : vm(other.vm), position(other.position), color (other.color) {}

    ParticleSpawner& operator=(const ParticleSpawner& other) {
        if (this != &other) {
            vm = other.vm;
            position = other.position;
            color = other.color;
        }
        return *this;
    }

    void Update(float deltaTime) {
        for (int i = 0; i < particles.size(); i++) {
            Particle& p = particles[i];
            p.life -= deltaTime;

            if (p.life > 0) {
                p.position.x += cosf(p.angle) * particleSpeed * deltaTime;
                p.position.y += sinf(p.angle) * particleSpeed * deltaTime;
            } else {
                particles.erase(particles.begin() + i);
                i--;
            }
        }

        if (spawnCount < maxParticles) {
            Vector2 particlePos = {
                position.x + cosf(GetRandomValue(-particleSpread, particleSpread)) * spawnDiameter,
                position.y + sinf(GetRandomValue(-particleSpread, particleSpread)) * spawnDiameter
            };
            float angle = atan2(particlePos.y - position.y, particlePos.x - position.x);
            Particle newParticle = { particlePos, color, angle, particleLifetime };
            particles.push_back(newParticle);
            spawnCount++;
        }
    }

    void Draw() {
        for (Particle& p : particles) {
            DrawCircleV(p.position, 1, p.color);
        }
    }
};