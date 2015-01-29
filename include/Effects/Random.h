#ifndef RANDOM_H
#define RANDOM_H

#include "Effects/Effect.h"

/*

    Random
    ------
    Random letter with random attribute filling the screen.

    This effect purpose is mostly for getting a quick template
    to make other effects.

*/

namespace Effects
{
    class Random : public Effects::Effect
    {
        public:
            Random(ConsoleEngine *engine) : Effects::Effect(engine) {}
            // ----------------------------------------------------------------------------
            // Draw - this is the main effect method
            // ----------------------------------------------------------------------------
            void Draw() {
                // We'll fill the console buffer with random data
                for (int y = 0; y < engine_->height(); y++) {
                    for (int x = 0; x < engine_->width(); x++) {
                        engine_->PutPixel(x, y, rand() % 256, rand() % 256);
                    }
                }
            }
    };
}


#endif // RANDOM_H
