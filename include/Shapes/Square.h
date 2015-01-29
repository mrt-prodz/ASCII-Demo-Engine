#ifndef SQUARE_H
#define SQUARE_H

#include "Shapes/Shape.h"

/*

    Square class
    ------------
    Class to draw square shape.

    Using 2 points:
        -Square with the same char
        -Square with specific bottom left, top left, bottom right, top right, horizontal and vertical char

    Using 1 point as center of square:
        -Square with the same char
        -Square with specific bottom left, top left, bottom right, top right, horizontal and vertical char

*/

namespace Shapes
{
    class Square : public Shape
    {
        private:
            // Draw square with same character
            void Draw(ConsoleEngine *engine, int &X1, int &Y1, int &X2, int &Y2, CHAR &letter, WORD &color) {
                for (int x = X1+1; x < X2; x++) {
                    for (int y = Y1; y <= Y2; y++) {
                        engine->PutPixel(x, Y1, letter, color);
                        engine->PutPixel(x, Y2, letter, color);
                        engine->PutPixel(X1, y, letter, color);
                        engine->PutPixel(X2, y, letter, color);
                    }
                }
            }

            // Draw square with specific characters for corners, horizontal and vertical lines
            void Draw(ConsoleEngine *engine, int &X1, int &Y1, int &X2, int &Y2, CHAR &cornerBotLeft,
                                                                                  CHAR &cornerTopLeft,
                                                                                  CHAR &cornerBotRight,
                                                                                  CHAR &cornerTopRight,
                                                                                  CHAR &lineHorizontal,
                                                                                  CHAR &lineVertical,
                                                                                  WORD &color) {
                for (int x = X1+1; x < X2; x++) {
                    for (int y = Y1; y <= Y2; y++) {
                        if (x > X1 && x < X2) {
                            engine->PutPixel(x, Y1, lineHorizontal, color);
                            engine->PutPixel(x, Y2, lineHorizontal, color);
                        }
                        if (y > Y1 && y < Y2) {
                            engine->PutPixel(X1, y, lineVertical, color);
                            engine->PutPixel(X2, y, lineVertical, color);
                        }
                        if (y == Y1) {
                            engine->PutPixel(X1, y, cornerTopLeft, color);
                            engine->PutPixel(X2, y, cornerTopRight, color);
                        }
                        if (y == Y2) {
                            engine->PutPixel(X1, y, cornerBotLeft, color);
                            engine->PutPixel(X2, y, cornerBotRight, color);
                        }
                    }
                }
            }
        protected:
        public:
            // 2 Points square
            // Draw a square with the same character
            Square(ConsoleEngine *engine, int X1, int Y1, int X2, int Y2, CHAR letter, WORD color) {
                if (X1 > X2) std::swap(X1, X2);
                if (Y1 > Y2) std::swap(Y1, Y2);
                Draw(engine, X1, Y1, X2, Y2, letter, color);
            }

            // 2 Points square
            // Draw a square specifying char for corners, horizontal and vertical lines
            // Square(engine, 0, 0, engine.width()-1, engine.height()-1, 200, 201, 188, 187, 205, 186, 0x0F);
            Square(ConsoleEngine *engine, int X1, int Y1, int X2, int Y2, CHAR cornerBotLeft,
                                                                           CHAR cornerTopLeft,
                                                                           CHAR cornerBotRight,
                                                                           CHAR cornerTopRight,
                                                                           CHAR lineHorizontal,
                                                                           CHAR lineVertical,
                                                                           WORD color) {
                if (X1 > X2) std::swap(X1, X2);
                if (Y1 > Y2) std::swap(Y1, Y2);
                Draw(engine,
                     X1,Y1,
                     X2,Y2,
                     cornerBotLeft,cornerTopLeft,cornerBotRight,cornerTopRight,lineHorizontal,lineVertical,
                     color);
            }

            // 1 Point as center of square
            // Draw a square with the same character from point X, Y as origin
            // Square(engine, (COORD){40, 20}, 79, 39, 254, 0x0F);
            Square(ConsoleEngine *engine, COORD pos, int WIDTH, int HEIGHT, CHAR letter, WORD color) {
                int X1 = pos.X - (WIDTH * 0.5);
                int X2 = pos.X + (WIDTH * 0.5);
                int Y1 = pos.Y - (HEIGHT * 0.5);
                int Y2 = pos.Y + (HEIGHT * 0.5);
                Draw(engine, X1, Y1, X2, Y2, letter, color);
            }

            // 1 Point as center of square
            // Draw a square specifying char for corners, horizontal and vertical lines
            // Square(engine, (COORD){40, 20}, 79, 39, 200, 201, 188, 187, 205, 186, 0x0F);
            Square(ConsoleEngine *engine, COORD pos, int WIDTH, int HEIGHT, CHAR cornerBotLeft,
                                                                             CHAR cornerTopLeft,
                                                                             CHAR cornerBotRight,
                                                                             CHAR cornerTopRight,
                                                                             CHAR lineHorizontal,
                                                                             CHAR lineVertical,
                                                                             WORD color) {
                int X1 = pos.X - (WIDTH * 0.5);
                int X2 = pos.X + (WIDTH * 0.5);
                int Y1 = pos.Y - (HEIGHT * 0.5);
                int Y2 = pos.Y + (HEIGHT * 0.5);
                Draw(engine,
                     X1,Y1,
                     X2,Y2,
                     cornerBotLeft,cornerTopLeft,cornerBotRight,cornerTopRight,lineHorizontal,lineVertical,
                     color);
            }

    };
}

#endif // SQUARE_H
