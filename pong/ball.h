#ifndef BALL_H
#define BALL_H

#include <iostream>

#include <raylib.h>

#include "consts.h"
#include "vectorMath.h"

class Ball {
public:
    int id;
    Vector2 position;
    float radius;
    Vector2 direction;
    float speed;
    Vector2 velocity;
    
    const float initialSpeed = 300.0f;
    const float maxSpeed = initialSpeed * 2;

    Ball();
    void Print();
    void Draw();
    void Update();

    Ball& operator=(const Ball &other) {
        return *this;
    }

    bool operator==(const Ball &other) const {
        return id == other.id;
    }

private:
    VectorMath & vm = VectorMath::getInstance();
    const Vector2 initialPosition = { HALF_W, HALF_H };

    static int nextId;
};

#endif