#ifndef TEXT_H
#define TEXT_H

#include "Composites/Composite.h"
#include "Assets/Font.h"

/*

    Text
    ----
    This class purpose is to write text using a font
    loaded from the asset manager.

    Constructors:
        - Output text at the center of the screen with optional color
          Text( pointer to console engine, path to font, text to write, optional color -default is 0x0F-)

        - Output text at X Y with optional color
          Text( pointer to console engine, path to font, text to write, X, Y, optional color -default is 0x0F-)


*/

namespace Composites
{
    class Text : public Composites::Composite
    {
        private:
            std::string text_; // Text to write
            std::string font_buffer_; // Stored text with font
            unsigned char color_; // Text color
            unsigned int text_width_; // Stored letters width
            unsigned int text_height_; // Stored letters height
            int offset_x_; // Offset if we want to place the text somewhere on screen
            int offset_y_; // Offset if we want to place the text somewhere on screen

            // Load font if the asset manager hasn't already
            void LoadFont(const char *path, const char *text) {
                // Load and store asset in asset manager
                if (engine_->asset_manager()->Load<Assets::Font>(path)) {
                    // If asset exists assign private member asset pointer
                    asset_ = engine_->asset_manager()->Get<Assets::Font>(path);
                    // Set text
                    SetText(text);
                    // Center text by default
                    Center();
                }
            }

        protected:
            Assets::Font *asset_ = 0;

        public:
            // Constructor - Display image at the center of the screen with optional color
            Text(ConsoleEngine *engine,
                 const char *path,
                 const char *text,
                 unsigned char color = 0x0F) : Composites::Composite(engine), color_(color) {
                LoadFont(path, text);
            }
            // Constructor - Display image at X, Y with optional color
            Text(ConsoleEngine *engine,
                 const char *path,
                 const char *text,
                 int X,
                 int Y,
                 unsigned char color = 0x0F) : Composites::Composite(engine), color_(color) {
                LoadFont(path, text);
                Move(X, Y);
            }
            // Center image in the middle of the screen
            void Center() {
                // If asset is assigned properly store variables
                if (asset_) {
                    // Calculate the final width and height of the text to be displayed
                    offset_x_ = (engine_->width()*0.5)-(text_width_*0.5);
                    offset_y_ = (engine_->height()*0.5)-(text_height_*0.5);
                }
            }
            // Move text at X, Y
            void Move(int X, int Y) {
                offset_x_ = X;
                offset_y_ = Y;
            }
            // Set text
            void SetText(const char *text) {
                // Check if font is successfully loaded first
                if (asset_) {
                    // Set variables
                    text_ = text;
                    // We will store temporary each letter here
                    Assets::Font::Letter *letter;
                    // Initialize stored text width
                    text_width_ = 0;
                    // We need to know upfront how wide the final text will be
                    for (unsigned int i=0; i < text_.size(); i++) {
                        letter = asset_->GetLetter(text_[i]);
                        if (letter) {
                            text_width_ += letter->width;
                        }
                    }
                    // All font characters must have same height
                    text_height_ = asset_->height();
                    // Prepare string size
                    font_buffer_.resize(text_width_*text_height_);
                    // Keep offset from the left
                    unsigned int x_offset = 0;
                    // Calculate offset and store text
                    for (unsigned int i=0; i < text_.size(); i++) {
                        letter = asset_->GetLetter(text_[i]);
                        if (letter) {
                            for (unsigned int y = 0; y < letter->height; y++) {
                                for (unsigned int x = 0; x < letter->width; x++) {
                                    // Update stored text
                                    font_buffer_[(x+x_offset)+(text_width_*y)] = letter->data[x+(letter->width*y)];
                                }
                            }
                            // Next letter will have an offset of the current letter width
                            // so it won't overlap
                            x_offset += letter->width;
                        }
                    }
                }
            }

            // ----------------------------------------------------------------------------
            // Draw - this is the main effect method, all effects should override this.
            // ----------------------------------------------------------------------------
            void Draw() {
                // If asset has been loaded properly draw it
                if (asset_) {
                    for (unsigned int y = 0; y < text_height_; y++) {
                        for (unsigned int x = 0; x < text_width_; x++) {
                            // Draw text using offset X and Y
                            const int x_offset = x + offset_x_;
                            const int y_offset = y + offset_y_;
                            if (x_offset >= 0 && x_offset < engine_->width() &&
                                y_offset >= 0 && y_offset < engine_->height() )
                                engine_->PutPixel(x_offset, y_offset, font_buffer_[x+(text_width_*y)], color_);
                        }
                    }
                }
            }

            // Return text width
            int text_width() const { return text_width_; }
            // Return text height
            int text_height() const { return text_height_; }
            // Return text offset X
            int offset_x() const { return offset_x_; }
            // Return text offset Y
            int offset_y() const { return offset_y_; }
    };
}

#endif // TEXT_H
