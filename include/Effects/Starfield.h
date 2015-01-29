#ifndef STARFIELD_H
#define STARFIELD_H

#include "Effects/Effect.h"

/*

    Starfield effect
    ----------------
    3D starfield effect.

*/

namespace Effects
{
    class Starfield : public Effects::Effect
    {
        private:
            static const short kMaxStars = 256; // Number of stars
            static const short kSpaceScale = 128; // Scale of the 'space'
            double speed_;
            short width_center_; // Center of the width
            short height_center_; // Center of the height
            struct Star3D {
                float x, y, z; // Position of the 3D star
            };
            Star3D stars_[kMaxStars];

            struct Star2D {
                float x, y; // Position of the 2D star
                CHAR letter; // Specify letter (to fake depth)
                WORD color; // Specify color (to fake depth)
            } star2d_;

            Star3D RandomStar(const int i, const int width, const int height) {
                Star3D star;
                // Since origin is the middle of the screen
                // we break the number of stars in 4 parts
                // depending on the value of index we position
                // the star on one of the part
                if (i < (kMaxStars/4)) {
                    star.x = -width + (rand() % width);
                    star.y = -height + (rand() % height);
                } else
                if (i > (kMaxStars/4) && i < (kMaxStars/3)) {
                    star.x = -width + (rand() % width);
                    star.y = rand() % height;
                } else
                if (i > (kMaxStars/3) && i < (kMaxStars/2)) {
                    star.x = rand() % width;
                    star.y = -height + (rand() % height);
                } else {
                    star.x = rand() % width;
                    star.y = rand() % height;
                }
                // Avoid stars coming directly toward the camera
                if (star.x == 0) star.x++;
                if (star.y == 0) star.y++;
                // Random Z value (clamped with kSpaceScale)
                star.z = rand() % kSpaceScale;
                return star;
            }
        public:
            Starfield(ConsoleEngine *engine, float speed = 40.0f) : Effects::Effect(engine, speed) {
                // Save middle of width and height
                width_center_ = engine_->width()  * 0.5;
                height_center_ = engine_->height() * 0.5;
                // Setup random position
                for (short i = 0; i < kMaxStars; i++) {
                    // Generate random star position
                    stars_[i] = RandomStar(i, width_center_, height_center_);
                }
            }

            // ----------------------------------------------------------------------------
            // Draw - this is the main effect method
            // ----------------------------------------------------------------------------
            void Draw() {
                speed_ = speed() * engine_->GetDeltaTime();
                // Animate stars
                for (short i = 0; i < kMaxStars; i++) {
                    // Translate 3d position to 2d position
                    star2d_.x = stars_[i].x * kSpaceScale / stars_[i].z + width_center_;
                    star2d_.y = stars_[i].y * kSpaceScale / stars_[i].z + height_center_;

                    // Different shade and shape depending on the depth
                    if (stars_[i].z < kSpaceScale/4) {
                        star2d_.letter = 43;
                        star2d_.color = 0x0F;
                    } else if (stars_[i].z > kSpaceScale/4 && stars_[i].z < kSpaceScale/3) {
                        star2d_.letter = 43;
                        star2d_.color = 0x07;
                    } else if (stars_[i].z > kSpaceScale/3 && stars_[i].z < kSpaceScale/2) {
                        star2d_.letter = 250;
                        star2d_.color = 0x08;
                    } else {
                        star2d_.letter = 250;
                        star2d_.color = 0x03;
                    }

                    // Check for screen boundaries before putting pixel
                    if (star2d_.x >= 0 && star2d_.x < engine_->width() &&
                        star2d_.y >= 0 && star2d_.y < engine_->height() )
                        // Place star on screen
                        engine_->PutPixel(star2d_.x, star2d_.y, star2d_.letter, star2d_.color);

                    // Move star, if Z is < 1 star is behind screen, make new pos
                    stars_[i].z -= speed_;
                    if (stars_[i].z < 1) {
                        stars_[i] = RandomStar(i, width_center_, height_center_);
                    }
                }
            }

    };
}

#endif // STARFIELD_H
