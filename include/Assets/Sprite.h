#ifndef SPRITE_H
#define SPRITE_H

#include "Assets/Asset.h"
#include <algorithm>

/*

    Sprite class
    ------------
    Class to load text files specifying an ASCII 'sprite'.

    Files must contain valid ASCII characters, parsing will get
    each character value. (no extended ASCII allowed)

    FORMAT NOTES:
        - Two first bytes are width and height
        - Newline and caret return are ignored
        - ASCII value of 255 will be transparent

    File format:
        [width][height]
        [char][char][char][char]etc..

*/

namespace Assets
{
    class Sprite : public Asset
    {
        private:
            unsigned char width_; // Sprite width
            unsigned char height_; // Sprite height
            std::string data_; // Sprite data

        public:
            Sprite(const char *path) : Asset(path) {
                // Load and parse file
                std::string content = Load();
                // Set parse status so the asset manager knows if it has to keep it
                parse_status_ = Parse(content);
            };

            // Parse file
            bool Parse(std::string &content) {
                if (content.size() < 2) {
                    LOG->error("Error with file data: %s", getPath()->c_str());
                    LOG->info("Invalid sprite size (less than 3 bytes)");
                    return false;
                }
                // Save width and height with the 2 first bytes
                width_  = static_cast <unsigned char> (content[0]);
                height_ = static_cast <unsigned char> (content[1]);
                // Remove \0 \r \n from content and resolution bytes
                for (unsigned int i=2; i<content.size(); i++) {
                    if (content[i] != 0xA && content[i] != 0xD && content[i] != 0x0) {
                        data_.push_back(content[i]);
                    }
                }
                // Check if content is over 2 bytes
                unsigned int cSize = data_.size();
                // Check if aData == width*height
                if (cSize != width_*height_) {
                    LOG->error("Error with file data: %s", getPath()->c_str());
                    LOG->info("Invalid sprite size (data != width*height)");
                    return false;
                }
                return true;
            }

            // Return sprite width (char numbers)
            unsigned char w() const { return width_; }
            // Return sprite height (char numbers)
            unsigned char h() const { return height_; }
            // Return sprite data
            const std::string *data() { return &data_; }
    };
}

#endif // ASSET_H
