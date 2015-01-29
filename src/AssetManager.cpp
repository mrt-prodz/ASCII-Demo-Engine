#include "AssetManager.h"

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
AssetManager::AssetManager() {
}

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
AssetManager::~AssetManager() {
    // Cleanup allocated memory
    Cleanup();
}

// ----------------------------------------------------------------------------
// Hash asset name into unsigned long (for hash table lookup)
// ----------------------------------------------------------------------------
unsigned long AssetManager::HashPath(const char *path) {
    unsigned long h = 0;
    while (*path)
        h = h << 1 ^ *path++;
    return h;
}

// ----------------------------------------------------------------------------
// Check if file exists and is not a directory
// ----------------------------------------------------------------------------
bool AssetManager::FileExists(const char *path) {
    DWORD dwAttrib = ::GetFileAttributes(path);
    return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

// ----------------------------------------------------------------------------
// Clean up memory
// ----------------------------------------------------------------------------
void AssetManager::Cleanup() {
    // Clean up allocated assets
    //for (std::unordered_map<unsigned long, Assets::Asset*>::const_iterator it = assets_.begin(); it != assets_.end(); ++it) {
    for (auto it = assets_.begin(); it != assets_.end(); ++it) {
        delete it->second;
    }
    assets_.clear();
}
