#ifndef WATERFALL_H
#define WATERFALL_H

#include "Effects/Effect.h"

/*

    Waterfall effect
    ----------------
    Drops falling from the top of the screen with a fading trail.
    Bottom of the screen is generating some noise to give the
    illusion of a splashing effect.

*/

namespace Effects
{
    class Waterfall : public Effects::Effect
    {
        private:
            static const int kMaxDrops = 1024; // Maximum number of water drops
            static const short kSpeed = 1; // Waterfall speed
            struct Drop {
                float x, y; // position of the star
            };
            Drop drops_[kMaxDrops];

        public:
            Waterfall(ConsoleEngine *engine) : Effects::Effect(engine) {
                // Setup random position
                for (int i = 0; i < kMaxDrops; i++) {
                    drops_[i].x = rand() % engine_->width();
                    drops_[i].y = rand() % engine_->height();
                }
            }

            // ----------------------------------------------------------------------------
            // Draw - this is the main effect method
            // ----------------------------------------------------------------------------
            void Draw() {
                // Animate falling drops
                for (int i = 0; i < kMaxDrops; i++) {
                    drops_[i].y += 0.75 * kSpeed;
                    if (drops_[i].y > engine_->height()) {
                        drops_[i].y = 0;
                        drops_[i].x = rand() % engine_->width();
                    }
                    // Add drop "trail"
                    engine_->PutPixel(drops_[i].x, drops_[i].y, 219, 0x0F);
                    if (drops_[i].y > 0)
                        engine_->PutPixel(drops_[i].x, drops_[i].y-1, 178, 0x07);
                    if (drops_[i].y > 1)
                        engine_->PutPixel(drops_[i].x, drops_[i].y-2, 178, 0x08);
                    if (drops_[i].y > 2)
                        engine_->PutPixel(drops_[i].x, drops_[i].y-3, 177, 0x03);
                    if (drops_[i].y > 3)
                        engine_->PutPixel(drops_[i].x, drops_[i].y-4, 176, 0x00);
                }
                // Splashing drops
                for (int i = 0; i < engine_->width()*0.15; i++) {
                    engine_->PutPixel(rand() * i % engine_->width(), engine_->height()-(rand() % engine_->height() * 0.10), 219, 0x0F);
                    engine_->PutPixel(rand() * i % engine_->width(), engine_->height()-(rand() % engine_->height() * 0.12), 178, 0x08);
                    engine_->PutPixel(rand() * i % engine_->width(), engine_->height()-(rand() % engine_->height() * 0.12), 177, 0x07);
                    engine_->PutPixel(rand() * i % engine_->width(), engine_->height()-(rand() % engine_->height() * 0.15), 176, 0x03);
                }
            }

    };
}

#endif // WATERFALL_H
