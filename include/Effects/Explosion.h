#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "Effects/Effect.h"
#include "Shapes/Circle.h"

/*

    Explosion effect
    ----------------
    Generating multiple circles with growing radius.
    Pixel color changing depending on the radius size.

*/

namespace Effects
{
    class Explosion : public Effects::Effect
    {
        private:
            static const short kMaxCircles = 32; // Number of circles
            static const short kSpaceScale = 256; // Scale of the 'space'
            static const short kSpeed = 1; // Speed of explosion
            short width_center_; // Center of the width
            short height_center_; // Center of the height

            struct Circle3D {
                float x, y, z; // Position of the 3D circle
            };
            Circle3D circles_[kMaxCircles];

            struct Circle2D {
                float x, y; // Position of the 2D circle
                CHAR letter; // Specify letter (to fake depth)
                WORD color; // Specify color (to fake depth)
            } circle2d_;

            // Generate circle and change color depending on radius (bigger == darker)
            void GenerateCircle(Circle2D &circle, const int radius, const int steps) {
                // Different shade and shape depending on the depth
                if (radius < 15) {
                    circle.letter = 219;
                    circle.color = 0x0F;
                } else
                if (radius > 15 && radius < 28) {
                    circle.letter = 254;
                    circle.color = 0x08;
                } else
                if (radius > 28 && radius < 48) {
                    circle.letter = 254;
                    circle.color = 0x08;
                } else {
                    circle.letter = 254;
                    circle.color = 0x03;
                }

                // Draw circle
                Shapes::Circle(engine_, circle.x, circle.y, circle.letter, circle.color, radius, steps);
            }

        public:
            Explosion(ConsoleEngine *engine) : Effects::Effect(engine) {
                // Save middle of width and height
                width_center_ = engine_->width()  * 0.5;
                height_center_ = engine_->height() * 0.5;
                Reset();
            }

            // Use the reset method if you want to use multiple times this effect
            void Reset() {
                // Setup random position
                for (short i = 0; i < kMaxCircles; i++) {
                    // Generate tunnel of circles (kMaxCircles / kSpaceScale ?)
                    circles_[i].x = width_center_*0.45 + rand() % 40;
                    circles_[i].y = height_center_*0.65 + rand() % 20;
                    circles_[i].z = kSpaceScale + (rand() % 3);
                }
            }
            // ----------------------------------------------------------------------------
            // Draw - this is the main effect method
            // ----------------------------------------------------------------------------
            void Draw() {
                // Animate particles
                for (short i = 0; i < kMaxCircles; i++) {
                    // Move particle, if Z is < 1 don't draw
                    if (circles_[i].z < 1) continue;
                    // Translate 3d position to 2d position
                    circle2d_.x = circles_[i].x * kSpaceScale / circles_[i].z;
                    circle2d_.y = circles_[i].y * kSpaceScale / circles_[i].z;

                    // Place circle on screen
                    GenerateCircle(circle2d_, kSpaceScale - circles_[i].z, 5 + rand() % 15);

                    // Move particle
                    circles_[i].z -= kSpeed;
                }
            }

    };
}

#endif // EXPLOSION_H
