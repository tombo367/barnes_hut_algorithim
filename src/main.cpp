#include "raylib.h"
#include "raymath.h"
#include "initialParticleGeneration.hpp"
#include "ParticleStruct.hpp"
#include "quadtree.hpp"
#include "Gravity.hpp"
#include <cmath>
#include <iostream>
#include <vector>

int main() {

    const int screenWidth = 1000;
    const int screenHeight = 1000;

    InitWindow(screenWidth, screenHeight, "barnes-hut simulation test");

    SetTargetFPS(60); 

    Boundary InitialBoundary(0.0, 0.0, 1000.0);

    const Vector2 centre = { 500.0f, 500.0f };
    const Vector2 centreOfMassVel = { 0.0f, 0.0f };
    const float charateristicRadius = { 300.0f };
    const int N = 100000;

    std::vector<Particle> ParticleList;

    for (int i = 0; i < N; i++) {
        Vector2 pos = initialPosition(centre, 50.0f);
        ParticleList.emplace_back(pos, initialVelocity(pos, centre, centreOfMassVel, N, charateristicRadius));
    }


    while (!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(BLACK);

            TreeNode root(InitialBoundary);

            std::vector<Vector2> positions;

            for (int i = 0; i < ParticleList.size(); i++) {
                ParticleList[i].updatePosition(GetFrameTime());
                if (Outside(ParticleList[i], InitialBoundary)) {
                    ParticleList.erase(ParticleList.begin() + i);
                    i--;
                    continue;
                }
                ParticleList[i].resetAcceleration();
                root.add(ParticleList[i]);
                positions.emplace_back(ParticleList[i].pos);
            }

            for (auto& p : ParticleList) {
                gravity(&p, &root);
                p.updateVelocity(GetFrameTime());
                p.Draw();
            }

            //DrawTreeNode(root);

            DrawFPS(10, 10);

        EndDrawing();
    }

};