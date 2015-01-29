#ifndef TEXTSCROLLER_H
#define TEXTSCROLLER_H

#include "Composites/Text.h"

/*

    Text Scroller
    -------------
    This is a derived class from Text and will change
    the offset over time.

    TODO: finish this and solve bug (duh..)
*/

namespace Composites
{
    class TextScroller : public Composites::Text
    {
        private:
            double speed_; // Scrolling speed
            int x_, y_;

        public:
            // Constructor - Display image at the center of the screen with optional color
            TextScroller(ConsoleEngine *engine,
                 const char *path,
                 const char *text,
                 double speed = 1.0f,
                 unsigned char color = 0x0F) : Composites::Text(engine, path, text, color), speed_(speed) {
                x_ = engine_->width() + text_width();
                y_ = offset_y();
            }
            // Constructor - Display image at X, Y with optional color
            TextScroller(ConsoleEngine *engine,
                 const char *path,
                 const char *text,
                 int X,
                 int Y,
                 double speed = 1.0f,
                 unsigned char color = 0x0F) : Composites::Text(engine, path, text, X, Y, color), speed_(speed) {}

            // ----------------------------------------------------------------------------
            // Draw - this is the main effect method, all effects should override this.
            // ----------------------------------------------------------------------------
            void Draw() {
                // Change offset over time
                // BUG - Why does it stop on the side of the screen?
                //X = offset_x() + speed_ * deltaTime(engine_->time_elapsed());
                //X = offset_x() - (2.0f * speed_ * deltaTime(engine_->time_elapsed()));
                x_ -= speed_;
                if (offset_x() < -text_width()) {
                    x_ = engine_->width() + text_width();
                }
                //LOG->info( "%d", X );
                Move(x_, y_);
                // Call parent draw method
                Text::Draw();
            }
    };
}

#endif // TEXTSCROLLER_H
