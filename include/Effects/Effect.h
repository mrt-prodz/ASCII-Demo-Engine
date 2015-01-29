#ifndef EFFECT_H
#define EFFECT_H

#include <cmath>

// Forward declaration of ConsoleEngine class
class ConsoleEngine;

/*

    Effect base class
    -----------------
    Inherit all effects from this class and make sure to override
    the Draw() method. (draw method will be called by ConsoleEngine)

*/

namespace Effects
{
    class Effect
    {
        protected:
            ConsoleEngine *engine_ = 0;

        private:
            float speed_;

        public:
            Effect(ConsoleEngine *engine, float speed = 20.0f) : engine_(engine),
                                                                 speed_(speed) {}
            virtual ~Effect() {};
            // ----------------------------------------------------------------------------
            // Draw - this is the main effect method, all effects should override this.
            // ----------------------------------------------------------------------------
            virtual void Draw() = 0;

            // Change speed of effect
            void speed(float speed) {
                speed_ = speed;
            }
            // Change speed of effect
            float speed() const {
                return speed_;
            }
    };
}

#endif // EFFECT_H
