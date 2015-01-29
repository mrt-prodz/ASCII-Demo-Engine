#ifndef TRANSITION_H
#define TRANSITION_H

#include "Effects/Effect.h"

/*

    Transition base class
    ---------------------
    This class purpose is for transitions between 2 or more
    effects. Inherit all transitions from this class.

    The reason we inherit from Effects::Effect is to be
    able to use the console engine sequencer and meet
    the fx member of the FX struct. (Effects::Effect)

    Simple transition example:
        - Draw first effect
        - Copy buffer from engine
        - Draw second effect
        - Combine both buffers and output result

    Effects will keep continuity if we use the same objects
    as the console engine sequencer.

*/

namespace Transitions
{
    class Transition : public Effects::Effect
    {
        private:
        protected:
        public:
            Transition(ConsoleEngine *engine) : Effects::Effect(engine) {};
            virtual ~Transition() {};
            // ----------------------------------------------------------------------------
            // Draw - this is the main effect method, all effects should override this.
            // ----------------------------------------------------------------------------
            virtual void Draw() = 0;
    };
}

#endif // TRANSITION_H
