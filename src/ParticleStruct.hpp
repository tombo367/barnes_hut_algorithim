#ifndef ParticleStruct 
#define ParticleStruct

#include "raylib.h"
#include "raymath.h"
#include <cmath>


const float radius = 1.0f;

struct Particle {
    
    Vector2 pos;
    Vector2 vel;
    Vector2 acc;

    Particle(Vector2 _pos, Vector2 _vel) : pos(_pos), vel(_vel){
        acc = Vector2Zero();
    }

    void updatePosition(float DeltaT) {
        pos = Vector2Add(pos, Vector2Scale(halfStepVelocity(DeltaT), DeltaT));
    }

    void updateVelocity(float DeltaT) {
        vel = Vector2Add(halfStepVelocity(DeltaT), Vector2Scale(acc, DeltaT / 2));
    }

    void updateAcceleration(Vector2 acceleration) {
        acc = Vector2Add(acc, acceleration);
    }

    void resetAcceleration() {
        acc = Vector2Zero();
    }

    void Draw() {
        DrawCircleV(pos, radius, WHITE);
    }

    bool operator==(const Particle& other) const {
        return this->pos.x == other.pos.x && this->pos.y == other.pos.y && this->vel.x == other.vel.x && this->vel.y == other.vel.y;
    }

private:
    Vector2 halfStepVelocity(float DeltaT) {
        return Vector2Add(vel, Vector2Scale(acc, DeltaT / 2));
    }

};



#endif