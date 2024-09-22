#ifndef initialParticleGeneration 
#define initialParticleGeneration

#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include <iostream>
#include <random>

Vector2 initialPosition(Vector2 centre, float charateristicRadius) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<float> radiusDis(1.0f / charateristicRadius);
    std::uniform_real_distribution<float> angleDis(0.0f, M_PI * 2.0f);

    float radius = radiusDis(gen);
    float angle = angleDis(gen);

    Vector2 pos = { radius * cos(angle), radius * sin(angle) };
    return Vector2Add(pos, centre);
}

float function(float r, int N, float charateristicRadius) {
    return N / charateristicRadius * exp( - r / charateristicRadius );
}

float integral(float maxR, int N, float charateristicRadius) {
    const int iterations = 100; 
    const float halfH = maxR / (2 * iterations);
    float r;
    float integrand = halfH * function(0.0f, N, charateristicRadius);

    for (int i = 1; i < iterations; i++) {
        r = halfH * 2.0f * i;
        integrand += halfH * 2 * function(r, N, charateristicRadius);
    }

    integrand += halfH * function(maxR, N, charateristicRadius);

    return integrand;

}

Vector2 initialVelocity(const Vector2 pos, const Vector2 centre, const Vector2 centreOfMassVel, const int N, float charateristicRadius) {

    const Vector2 position = Vector2Subtract(pos, centre);
    const Vector2 radialVector = Vector2Normalize(position);
    const float radius = Vector2Length(position);

    const float speed = sqrt(1.0f * integral(radius, N, charateristicRadius) / radius);
    const Vector2 velDirection = { -radialVector.y, radialVector.x};
    Vector2 vel = Vector2Scale(velDirection, speed);

    vel = Vector2Add(vel, centreOfMassVel);

    return vel;
}

#endif