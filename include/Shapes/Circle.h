#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shapes/Shape.h"

/*

    Circle class
    ------------
    Class to draw circle shape.

*/

namespace Shapes
{
    class Circle : public Shape
    {
        private:
        protected:
        public:
            Circle(ConsoleEngine *engine, const int CircleX, const int CircleY,
                   const CHAR letter, const WORD color, const int radius, const int steps) {
                int x, y;
                for (int i = 0; i < 360; i += steps) {
                    x = CircleX + radius * cos(i);
                    y = CircleY + radius * sin(i);

                    // Check for screen boundaries before putting pixel
                    if (x >= 0 && x < engine->width() && y >= 0 && y < engine->height() )
                        engine->PutPixel(x, y, letter, color);
                }
            }

    };
}

#endif // CIRCLE_H
