#ifndef LINE_H
#define LINE_H

#include "Shapes/Shape.h"

/*

    Line class
    ----------
    Class to draw line shape using Bresenham's line algorithm.

*/

namespace Shapes
{
    class Line : public Shape
    {
        private:
        protected:
        public:
            // Draw a line from point 1 to point 2
            Line(ConsoleEngine *engine, int X1, int Y1, int X2, int Y2, CHAR letter, WORD color) {
            }
    };
}

#endif // SQUARE_H
