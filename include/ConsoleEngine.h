#ifndef CONSOLEENGINE_H
#define CONSOLEENGINE_H

#include <cstdlib>
#include <vector>
#include <windows.h>
#include <tchar.h>

#include "Logging.h"
#include "AssetManager.h"
#include "Effects/Effect.h"

/*

    ConsoleWindow class
    -------------------
    This is the console window manager, all effects will draw themselves in the
    consoleBuffer member.

    This class use a vector of FX struct as a sequencer. Effects are added specifying
    when they should play and for how long. (FX struct {&fx, time_start, time_duration} )

    We can pause (P), resume (R) or quit the demo. (ESC)

    time_elapsed() for current demo time (minus paused time)
    [Use libxmp module library timer to synchronize ?]

*/

class ConsoleEngine
{
    private:
        HANDLE write_handle_; // Handle to write to the console.
        HANDLE read_handle_; // Handle to read to the console.
        DWORD number_events_; // Number of events with the read handle
        DWORD number_events_read_; // Number of events we have read
        CHAR_INFO *console_buffer_ = 0; // Console buffer where we will draw our effects
        SMALL_RECT write_area_; // Write region
        COORD char_buffer_size_; // Buffer size (cols, rows)
        COORD char_pos_; // Coordinate of the upper left corner (col, row)
        short width_;  // Window width
        short height_; // Window height
        bool running_; // Flag to stop main loop
        bool paused_; // Flag to pause the loop
        double time_elapsed_;
        DWORD start_tick_;
        DWORD paused_tick_;
        float old_time_; // Store old time here
        float new_time_; // Store new time here
        AssetManager *asset_manager_ = 0; // Asset Manager pointer
        // FX struct for the ConsoleEngine sequencer
        struct FX {
            // Pointer to Effect class
            Effects::Effect *fx;
            // time to start effect, duration to keep the effect on screen
            double time_start, time_duration;
        };

        // Set elapsed time (called with each Update)
        void SetTimeElapsed();
        // Display or hide console cursor
        void DisplayCursor(const bool value);
        // Clean up memory
        void Cleanup();

    public:
        // Constructor
        ConsoleEngine();
        // Destructor
        ~ConsoleEngine();
        // Initialize window (handles, title, dimension, etc..)
        bool Init(const TCHAR * title, const short width, const short height);
        // Check for key events for the main loop
        void CheckEvent();
        // Update the data of console buffer with effects
        void Update();
        // Redraw screen with the data of console buffer
        void Render();
        // Return elapsed time
        double time_elapsed() const;
        // Return delta time
        float GetDeltaTime();
        // Put "pixel" in console buffer at x y pos with style
        void PutPixel(const int X, const int Y, const CHAR letter, const WORD attribute);
        // Put "pixel" in console buffer at index pos with style
        void PutPixel(const int index, const CHAR letter, const WORD attribute);
        // Get "pixel" from console buffer at pos (return char and attribute)
        void GetPixel(const int X, const int Y, CHAR_INFO *pixel);
        // Clear buffer
        void Clear(CHAR letter = ' ', WORD color = 0);
        // Add effect to sequencer
        void AddEffect(Effects::Effect *fx, double time_start, double time_duration);
        // Return if loop is running
        bool running() const { return running_; }
        // Return if loop is paused
        bool paused() const { return paused_; }
        // Return write console handle
        HANDLE write_handle() const { return write_handle_; }
        // Return read console handle
        HANDLE read_handle() const { return read_handle_; }
        // Return console window width
        short width() const { return width_; }
        // Return console window height
        short height() const { return height_; }
        // Return console buffer
        CHAR_INFO *console_buffer() const { return console_buffer_; }
        // Return asset manager
        AssetManager *asset_manager() const { return asset_manager_; }

    protected:
        std::vector<ConsoleEngine::FX> effects_; // FX struct {Pointer to effect, time_start, time_duration}
};

#endif // CONSOLEENGINE_H
