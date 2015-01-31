#ifndef FONT_H
#define FONT_H

#include "Assets/Asset.h"

/*

    Font
    ----
    Class to load text files specifying an ASCII 'font'.

    Files must contain valid ASCII characters, parsing will get
    each character value. (no extended ASCII allowed)

    FORMAT NOTES:
        - first byte is total number of characters
        - char struct
            - First byte is character value
            - Two next bytes are width and height
            - width*height character representation
        - Newline and caret return are ignored
        - ASCII value of 255 will be transparent

    File format:
        [total number of characters]
        [character][width][height]
        [char][char][char][char]
        etc..

*/

namespace Assets
{
    class Font : public Asset
    {
        public:
            Font(const char *path) : Asset(path) {
                // Load and parse file
                std::string content = Load();
                // Set parse status so the asset manager knows if it has to keep it
                parse_status_ = Parse(content);
            };

            // Struct to hold Letter
            struct Letter {
                unsigned char letter;
                unsigned char width; // Font character width
                unsigned char height; // Font character height
                std::string data; // Font character data
            };

            // Parse file
            bool Parse(std::string &content) {
                if (content.size() < 2) {
                    LOG->error("Error with file data: %s", getPath()->c_str());
                    LOG->info("Invalid sprite size (less than 3 bytes)");
                    return false;
                }
                // First byte of the file is the number of characters in our font file
                unsigned int charNum = static_cast <unsigned char> (content[0]);
                // Keep offset
                unsigned int offset = 1;
                unsigned int csize = 0;
                // Start parsing all characters
                for (unsigned int i=0; i<charNum; i++) {
                    // First bytes shouldn't be 0xA 0xD 0x20
                    while(content[offset] == 0xA || content[offset] == 0xD) {
                        offset++;
                    }
                    // Create struct to store letter
                    Letter c;
                    // First byte of the character is the letter itself
                    c.letter = static_cast <unsigned char> (content[offset++]);
                    // Second byte is width
                    c.width  = static_cast <unsigned char> (content[offset++]);
                    // Third byte is height
                    c.height = static_cast <unsigned char> (content[offset++]);
                    // Total number of chars to store in c.data
                    csize = c.width*c.height;
                    //LOG->info("Processing letter: %c %X - w: %02d | h: %02d - offset: %u - total: %u", c.letter, c.letter, c.width, c.height, offset-2, c.width*c.height);
                    // Remove \0 \r \n from cleanContent and resolution bytes
                    unsigned char byte;
                    while (csize) {
                        if (offset > content.size()) {
                            LOG->error("Error while parsing font file: %s", getPath()->c_str());
                            std::string parsedLetters;
                            for (auto it=letters_.begin(); it!=letters_.end(); it++) {
                                parsedLetters += it->second.letter;
                            }
                            LOG->info("Parsed letters: %d - %s", i, parsedLetters.c_str());
                            return false;
                        }
                        // Store byte temporary
                        byte = content[offset];
                        // If byte is not a newline, carriage return or null byte store it
                        if (byte != 0xA && byte != 0xD && byte != 0x0) {
                            c.data.push_back(byte);
                            csize--;
                        }
                        // Keep on increasing offset of which byte we are currently reading
                        offset++;
                    }
                    // Store finished letter
                    letters_.insert(std::make_pair(c.letter, c));
                }
                // Store font height by using first character data
                height_ = letters_.begin()->second.height;

                return true;
            }

            // Return number of characters in font
            unsigned int NumberOfLetters() const { return letters_.size(); }
            // Return font height
            unsigned int height() const { return height_; }
            // Return all font letters
            const std::map<unsigned char, Letter> *letters() { return &letters_; }
            // Return character
            Letter *GetLetter(unsigned char c) {
                if (letters_.find(c) == letters_.end()) return 0;
                else return &letters_[c];
            }

        private:
            std::map<unsigned char, Letter> letters_; // Letters will be stored in this map
            unsigned int height_; // Font height;

            // Debugging
            void Debug() {
                for (auto it=letters_.begin(); it!=letters_.end(); it++) {
                    printf("%c\n", it->second.letter);
                    for (unsigned char y = 0; y < it->second.height; y++) {
                        for (unsigned char x = 0; x < it->second.width; x++) {
                            printf("%c", it->second.data[x+(it->second.width*y)]);
                        }
                        printf("\n");
                    }
                }
            }

    };
}

#endif // FONT_H
