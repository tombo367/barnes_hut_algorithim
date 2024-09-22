#ifndef Gravity 
#define Gravity

#include "raylib.h"
#include "raymath.h"
#include "ParticleStruct.hpp"
#include "quadtree.hpp"

const float theta = 10.0;

Vector2 gravitationalAcceleration(Vector2 a, Vector2 b, int m_b) {
    Vector2 displacementVector = Vector2Subtract(a, b);
    float squareDistance = Vector2LengthSqr(displacementVector);
    float minSquareDistance = pow(radius, 2);
    if (squareDistance <= minSquareDistance) {
        return Vector2Scale(Vector2Normalize(displacementVector), - (m_b * 1.0f) / minSquareDistance);
    }
    else {
        return Vector2Scale(Vector2Normalize(displacementVector), - (m_b * 1.0f) / squareDistance);
    }
}

void gravity(Particle* particle, TreeNode* treenode) {
    if (treenode->leaf) {
        if (treenode->filled) {
            const Particle& nodeParticle = treenode->getParticle();
            if (particle != &nodeParticle) {
                particle->updateAcceleration(gravitationalAcceleration(particle->pos, nodeParticle.pos, 1.0));
            }
        }
        return;
    }

    if ((treenode->boundary.w / Vector2Distance(particle->pos, treenode->TotalCentre)) < theta) {
        particle->updateAcceleration(gravitationalAcceleration(particle->pos, treenode->TotalCentre, treenode->TotalMass));
        return;
    }

    for (auto& child : treenode->children) {
        gravity(particle, &child);
    }
}

#endif