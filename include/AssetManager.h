#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <unordered_map>
#include <windows.h>

#include "Logging.h"
#include "Assets/Asset.h"

/*

    AssetManager class
    ------------------
    This is the asset manager class, all assets will be loaded and accessed from here.

    Store asset:
        AssetManager::Load <Assets::Sprite> ("assets/sprites/logo.txt")

    Get asset from stored assets:
        AssetManager::Get <Assets::Sprite> ("assets/sprites/logo.txt")

*/

class AssetManager
{
    private:
        // Return hash of the path for the unordered map lookup table
        unsigned long HashPath(const char *path);
        // Check if file exists and is not a directory
        bool FileExists(const char *path);
        // Clean up memory
        void Cleanup();

    public:
        // Constructor
        AssetManager();
        // Destructor
        ~AssetManager();

        // Load and store asset into unordered map using a hash of its path
        template <class T>
        bool Load(const char *path) {
            // If file doesn't exists return false
            if (!FileExists(path)) {
                //std::printf("[!] Error while loading asset: %s\n", path);
                LOG->error("Error while loading asset: %s", path);
                return false;
            }
            // Check if asset hash already in stored assets
            unsigned long hashedPath = HashPath(path);
            auto it = assets_.find(hashedPath);
            if (it != assets_.end()) {
                LOG->info("Asset %s already stored", path);
                return true;
            }

            // Try loading Asset
            T *asset = new T(path);
            // Check if asset has loaded and parsed properly
            if (!asset || !asset->parsed()) {
                LOG->error("Error while parsing asset: %s", path);
                delete asset;
                return false;
            }
            // File exists and Asset successfully loaded - store it
            assets_.insert( std::make_pair(hashedPath, asset) );
            return true;
        }

        // Get a pointer to an asset from stored map (using the hashed path)
        template <class T>
        T *Get(const char *path) {
            auto it = assets_.find(HashPath(path));
            if (it == assets_.end()) {
                LOG->error("Asset Manager cannot find : %s", path);
                return NULL;
            } else {
                return static_cast <T*> (it->second);
            }
        }

    protected:
        std::unordered_map<unsigned long, Assets::Asset*> assets_; // store all assets here
};

#endif // ASSETMANAGER_H
