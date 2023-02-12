#ifndef VECTORMATH_H
#define VECTORMATH_H

#include <raylib.h>

#include <iostream>
#include <math.h>
#include <cmath>

class VectorMath {
    public:
    static VectorMath& getInstance() {
        static VectorMath instance;
        return instance;
    }

    Vector2 Add(Vector2 one, Vector2 two) {
        return {one.x + two.x, one.y + two.y};
    }

    Vector2 Subtract(Vector2 one, Vector2 two) {
        return {one.x - two.x, one.y - two.y};
    }

    Vector2 Multiply(Vector2 one, Vector2 two) {
        return {one.x * two.x, one.y * two.y};
    }

    Vector2 Divide(Vector2 one, Vector2 two) {
        return {one.x / two.x, one.y / two.y};
    }

    Vector2 Scale(Vector2 vector, float scalar)
    {
        return { vector.x * scalar, vector.y * scalar };
    }

    Vector2 Rotate(Vector2 vector, float rad)
    {
        return { vector.x * cosf(rad) - vector.y * sinf(rad),
                 vector.x * sinf(rad) + vector.y * cosf(rad) };
    }

    Vector2 Normalize(Vector2 vector) {
        float magnitude = sqrtf(vector.x * vector.x + vector.y * vector.y);
        return {vector.x / magnitude, vector.y / magnitude};
    }   

    Vector2 Clamp(Vector2 value, Vector2 min, Vector2 max) {
        Vector2 result = {
            value.x < min.x ? min.x : value.x > max.x ? max.x : value.x,
            value.y < min.y ? min.y : value.y > max.y ? max.y : value.y
        };
        return result;
    }

    Vector2 Reflect(Vector2 direction, Vector2 surfaceNormal) {
        return Subtract(direction, Scale(surfaceNormal, 2 * DotProduct(direction, surfaceNormal)));
    }

    // Direction
    Vector2 GetRandomDirection() {        
        // Generate a random angle in degrees
        float angle = rand() % 360;
        
        // Convert the angle to radians
        angle = angle * M_PI / 180.0f;
        
        // Calculate the direction vector
        return {cos(angle), sin(angle)};
    }

    // Translate
    Vector2 GetMinimumMovingTranslation(Vector2 position, float radius, Rectangle rect, Vector2 next) {
        Vector2 mtd = {0, 0};

        // Get the closest point on the rectangle to the center of the circle
        Vector2 closest = Clamp(position, {next.x, next.y}, Add({next.x, next.y}, {(float) rect.width, (float) rect.height}));
        
        // Get the distance between the closest point and the center of the circle
        Vector2 distance = Subtract(position, closest);
        
        // If the distance is less than the radius of the circle, there is overlap
        float d = Length(distance);
        if (d < radius) {
            // Calculate the MTD by normalizing the distance and scaling it by the distance minus the radius of the circle
            mtd = Scale(Normalize(distance), radius - d);
        }

        if(std::isnan(mtd.x)) mtd.x = 0;
        if(std::isnan(mtd.y)) mtd.y = 0;

        return mtd;
    }

    Vector2 GetMinimumTranslation(Vector2 position, float radius, Rectangle rect) {
        Vector2 mtd = {0, 0};

        // Get the closest point on the rectangle to the center of the circle
        Vector2 closest = Clamp(position, {rect.x, rect.y}, Add({rect.x, rect.y}, {(float) rect.width, (float) rect.height}));
        
        // Get the distance between the closest point and the center of the circle
        Vector2 distance = Subtract(position, closest);
        
        // If the distance is less than the radius of the circle, there is overlap
        float d = Length(distance);
        if (d < radius) {
            // Calculate the MTD by normalizing the distance and scaling it by the distance minus the radius of the circle
            mtd = Scale(Normalize(distance), radius - d);
        }

        if(std::isnan(mtd.x)) mtd.x = 0;
        if(std::isnan(mtd.y)) mtd.y = 0;

        return mtd;
    }

    // Float
    float DotProduct(Vector2 a, Vector2 b) {
        return a.x * b.x + a.y * b.y;
    }

    float Clamp(float value, float min, float max) {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    float Length(Vector2 vector) {
        return sqrtf(vector.x * vector.x + vector.y * vector.y);
    }

    float ConstrainAngle(float x){
        x = fmod(x,360);
        if (x < 0)
            x += 360;
        return x;
    }

    float GetRandomValue(float min, float max)
    {
        return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
    }

    // Range
    bool isWithinRange(float a, float b, float range) {
        return (a > b - range) && (a < b + range);
    }

    // Decimals
    float roundToFirstDecimalPlace(float num) {
        return roundf(num * 10.0f) / 10.0f;
    }

    private:
    VectorMath() {}
    VectorMath(VectorMath const&) = delete;
    void operator=(VectorMath const&) = delete;
};

#endif
