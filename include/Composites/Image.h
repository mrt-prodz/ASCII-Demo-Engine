#ifndef IMAGE_H
#define IMAGE_H

#include "Composites/Composite.h"
#include "Assets/Sprite.h"

/*

    Image class
    -----------
    This class purpose is to display a sprite or any
    similar class with width / height / data members.

    Constructors:
        - Output image at the center of the screen
          Image( pointer to console engine, path to image)

        - Output image at X Y
          Image( pointer to console engine, path to image, text to write, X, Y)

    TODO: Add constructor for image containing data for colors as well?

*/

namespace Composites
{
    class Image : public Composites::Composite
    {
        private:
            Assets::Sprite *asset_ = 0;
            short offset_x_;
            short offset_y_;
            unsigned char pixel_;

            void LoadImage(const char *path) {
                // Load and store asset in asset manager
                if (engine_->asset_manager()->Load<Assets::Sprite>(path)) {
                    // If asset exists assign private member asset pointer
                    asset_ = engine_->asset_manager()->Get<Assets::Sprite>(path);
                    // Center image by default
                    Center();
                }
            }
        protected:
        public:
            // Constructor - Display image at the center of the screen
            Image(ConsoleEngine *engine, const char *path) : Composites::Composite(engine) {
                LoadImage(path);
            }
            // Constructor - Display image at X, Y
            Image(ConsoleEngine *engine, const char *path, short X, short Y) : Composites::Composite(engine) {
                LoadImage(path);
                Move(X, Y);
            }
            // Center image in the middle of the screen
            void Center() {
                // If asset is assigned properly store variables
                if (asset_) {
                    offset_x_ = (engine_->width()*0.5)-(asset_->w()*0.5);
                    offset_y_ = (engine_->height()*0.5)-(asset_->h()*0.5);
                }
            }
            // Move image at X, Y
            void Move(short X, short Y) {
                offset_x_ = X;
                offset_y_ = Y;
            }
            // ----------------------------------------------------------------------------
            // Draw - this is the main effect method, all effects should override this.
            // ----------------------------------------------------------------------------
            void Draw() {
                // If asset has been loaded properly draw it
                if (asset_)
                    for (short y = 0; y < asset_->h(); y++)
                        for (short x = 0; x < asset_->w(); x++) {
                            pixel_ = asset_->data()->at(x+(asset_->w()*y));
                            if (pixel_ < 255)
                                engine_->PutPixel(x+offset_x_, y+offset_y_, pixel_, 0x0F);
                        }
            }
    };
}

#endif // IMAGE_H
