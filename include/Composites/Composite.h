#ifndef COMPOSITE_H
#define COMPOSITE_H

#include "Effects/Effect.h"

/*

    Composite base class
    --------------------
    This class purpose is for 'post production' effects.
    Inherit all composites from this class.

    The reason we inherit from Effects::Effect is to be
    able to use the console engine sequencer and meet
    the fx member of the FX struct. (Effects::Effect)

*/

namespace Composites
{
    class Composite : public Effects::Effect
    {
        private:
        protected:
        public:
            Composite(ConsoleEngine *engine) : Effects::Effect(engine) {};
            virtual ~Composite() {};
            // ----------------------------------------------------------------------------
            // Draw - this is the main effect method, all effects should override this.
            // ----------------------------------------------------------------------------
            virtual void Draw() = 0;
    };
}

#endif // COMPOSITE_H
