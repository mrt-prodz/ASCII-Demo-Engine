#ifndef ASSET_H
#define ASSET_H

#include <map>

/*

    Asset base class
    -----------------
    Inherit all assets from this class. The asset manager will store all
    assets in an unordered map and access specific asset by their hashed
    path. (hash lookup table)

    Set parse status in derived class or else the asset manager will not
    store it.

*/

namespace Assets
{
    class Asset
    {
        private:
            const std::string path_; // Path of asset for debugging purpose

        protected:
            bool parse_status_; // Flag to specify if file has been parsed correctly

        public:
            Asset(const char *path) : path_(path), parse_status_(false) {};
            virtual ~Asset() {};

            // Load file
            std::string Load() {
                // Open file and return content
                std::FILE *fp = std::fopen(path_.c_str(), "r");
                if (fp) {
                    std::string content;
                    std::fseek(fp, 0, SEEK_END);
                    content.resize(std::ftell(fp));
                    std::rewind(fp);
                    std::fread(&content[0], 1, content.size(), fp);
                    std::fclose(fp);
                    return content;
                }
                LOG->error("Error while loading file: %s", path_.c_str());
                return 0;
            }

            // File parser - derived class should override this
            virtual bool Parse(std::string &content) { return false; };
            // Return path string
            const std::string *getPath() { return &path_; }
            // Return parse status
            bool parsed() const { return parse_status_; }
    };
}

#endif // ASSET_H
