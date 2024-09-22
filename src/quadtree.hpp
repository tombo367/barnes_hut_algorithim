#ifndef quadtree 
#define quadtree

#include "raylib.h"
#include "raymath.h"
#include "ParticleStruct.hpp"
#include <cmath>
#include <iostream>
#include <queue>
#include <vector>

struct Boundary {
    float x, y;
    float w;

    Boundary(float _x, float _y, float _w) : x(_x), y(_y), w(_w) {
    }

    bool inside(Particle _particle) {
        if (_particle.pos.x >= x && _particle.pos.x < x + w && _particle.pos.y >= y && _particle.pos.y < y + w) {
            return true;
        }
        else {
            return false;
        }
    }
};

struct TreeNode {

    Boundary boundary;
    bool leaf = true;
    bool filled = false;
    Vector2 TotalCentre = Vector2Zero();
    float TotalMass = 0.0f;

    std::vector<Particle> ParticleList;
    std::vector<TreeNode> children;

    TreeNode(const Boundary& _boundary) : boundary(_boundary){
    }

    void add(const Particle& _particle) {
        
        // check to see whether the current node is a leaf
        if (leaf) {
            // if filled, subdivide and add particles to the children nodes
            if (filled) {
                subdivide();
                addToChildren(_particle);
                addToChildren(ParticleList[0]);
                ParticleList.clear();
                leaf = false;
            }
            // if empty, simply add particle to this TreeNode
            else {
                ParticleList.emplace_back(_particle);
                filled = true;
            }
        }

        // if not a leaf then check and add particle to correct childnode
        else {
            addToChildren(_particle);
        }
        updateTotalCentreAndMasss(_particle);
    }

    Particle getParticle() {
        return ParticleList[0];
    }


private:
    void subdivide() {
        float halfW = boundary.w / 2;

        children = {
            TreeNode(Boundary(boundary.x + halfW, boundary.y, halfW)),
            TreeNode(Boundary(boundary.x, boundary.y, halfW)),
            TreeNode(Boundary(boundary.x + halfW, boundary.y + halfW, halfW)),
            TreeNode(Boundary(boundary.x, boundary.y + halfW, halfW))
        };
    }

    void addToChildren(const Particle& _particle) {
        for (auto& child : children) {
            if (child.boundary.inside(_particle)) {
                child.add(_particle);
                break;
            }
        }
    }

    void updateTotalCentreAndMasss(Particle NewParticle) {
        TotalCentre = Vector2Scale(Vector2Add(Vector2Scale(TotalCentre, TotalMass), NewParticle.pos), 1 / (TotalMass + 1));
        TotalMass += 1.0f;
    }
};

void DrawTreeNode(TreeNode tn) {
    DrawRectangleLines(tn.boundary.x, tn.boundary.y, tn.boundary.w, tn.boundary.w, WHITE);
    for (auto& child : tn.children) {
        DrawTreeNode(child);
    }
}

bool Outside(Particle _particle, Boundary _boundary) {
    if (_particle.pos.x < _boundary.x || _particle.pos.y < _boundary.y || _particle.pos.x > _boundary.w || _particle.pos.y > _boundary.w) return true;
    else return false;
}

#endif