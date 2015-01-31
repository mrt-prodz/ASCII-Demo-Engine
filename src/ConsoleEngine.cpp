#include "ConsoleEngine.h"

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
ConsoleEngine::ConsoleEngine() : running_(true), paused_(false) {
    // Initialize seed
    srand(54352);
}

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
ConsoleEngine::~ConsoleEngine() {
    // Cleanup allocated memory
    Cleanup();
    // Show cursor back again
    DisplayCursor(true);
}

// ----------------------------------------------------------------------------
// Clean up memory
// ----------------------------------------------------------------------------
void ConsoleEngine::Cleanup() {
    // Delete allocated asset manager
    delete asset_manager_;
    // Delete allocated console buffer
    delete[] console_buffer_;
}

// ----------------------------------------------------------------------------
// Init window / title / size
// ----------------------------------------------------------------------------
bool ConsoleEngine::Init(const TCHAR * title, const short width, const short height) {
    // Store width and height
    width_ = width;
    height_ = height;

    // Get write handle of console window
    write_handle_ = ::GetStdHandle(STD_OUTPUT_HANDLE);
    if (!write_handle_) {
        LOG->error("Error while getting write handle of console window");
        return false;
    }

    // Get read handle of console window
    read_handle_ = ::GetStdHandle(STD_INPUT_HANDLE);
    if (!read_handle_) {
        LOG->error("Error while getting read handle of console window");
        return false;
    }

    // Change the window title:
    if (!::SetConsoleTitle(title)) {
        LOG->error("Error while setting console window title");
        return false;
    }

    // Hide blinking cursor
    DisplayCursor(false);

    // Change the console window size and store write area
    write_area_ = (SMALL_RECT){0, 0, short(width-1), short(height-1)};
    if (!::SetConsoleWindowInfo(write_handle_, TRUE, &write_area_)) {
        LOG->error("Error while setting console window size");
        return false;
    }

    // Change the internal buffer size
    char_buffer_size_ = (COORD){width, height};
    if (!::SetConsoleScreenBufferSize(write_handle_, char_buffer_size_)) {
        LOG->error("Error while setting console window buffer size");
        return false;
    }

    // Set console buffer size and set values to 0
    console_buffer_ = new CHAR_INFO[width * height];
    Clear();

    // Set character position for redraw()
    char_pos_ = (COORD){0, 0};

    // Create asset manager
    asset_manager_ = new AssetManager();

    // Init timer related variables
    start_tick_ = ::GetTickCount();
    old_time_ = 0.0;
    new_time_ = 0.0;

    return true;
}

// ----------------------------------------------------------------------------
// Check for key events for the main loop
// ----------------------------------------------------------------------------
void ConsoleEngine::CheckEvent() {
    // Get number of events and store in numEvents
    ::GetNumberOfConsoleInputEvents(read_handle_, &number_events_);
    if (number_events_) {
        // Create buffer of the size of numEvents
        INPUT_RECORD *eventBuffer = new INPUT_RECORD[number_events_];
        // Read events into eventBuffer and save how many we have read into numEventsRead
        ::ReadConsoleInput(read_handle_, eventBuffer, number_events_, &number_events_read_);
        // Cycle through events
        for (DWORD i = 0; i < number_events_read_; i++) {
            // Check for key event
            if (eventBuffer[i].EventType == KEY_EVENT) {
                // Check which key has been pressed
                switch(eventBuffer[i].Event.KeyEvent.wVirtualKeyCode) {
                    // Escape key = Quit
                    case VK_ESCAPE:
                        // Tell main loop to stop
                        running_ = false;
                        break;
                    // Space key = Restart
                    case VK_SPACE:
                        if (paused_) return;
                        // Run again to restart
                        //Run();
                        start_tick_ = ::GetTickCount();
                        paused_tick_ = start_tick_;
                        break;
                    // P key = Pause
                    case 'P':
                        if (paused_) return;
                        // Tell main loop to pause
                        paused_ = true;
                        // Set paused tick to calculate elapsed time properly
                        paused_tick_ = ::GetTickCount();
                        break;
                    // R key = Resume
                    case 'R':
                        if (!paused_) return;
                        // Tell main loop to resume
                        paused_ = false;
                        // Update start tick with paus tick to get correct elapsed time
                        start_tick_ += ::GetTickCount() - paused_tick_;
                        break;
                }
            }
        }
        // Delete buffer of numEvents
        delete[] eventBuffer;
    }
}

// ----------------------------------------------------------------------------
// Update buffer data with vector of effects (sequencer)
// ----------------------------------------------------------------------------
void ConsoleEngine::Update() {
    // Update time elapsed
    SetTimeElapsed();
    // Check for paused state before updating
    if (paused_) return;
    // Clear buffer
    Clear();
    // Cycle all effects and draw effect within time range (time_start and time_duration)
    for (auto it = effects_.begin(); it != effects_.end(); it++) {
        // If effect time start and time duration is in the current time elapsed frame draw effect
        if (it->time_start <= time_elapsed() && (it->time_start + it->time_duration) >= time_elapsed()) {
            // Call draw method of effect
            it->fx->Draw();
        }
    }
}

// ----------------------------------------------------------------------------
// Redraw screen using consoleBuffer
// ----------------------------------------------------------------------------
void ConsoleEngine::Render() {
    // Check for paused state before rendering
    if (paused_) return;
    // While all effects affect only the console buffer
    // it's not until now that we display it
    ::WriteConsoleOutputA(write_handle_, console_buffer_, char_buffer_size_, char_pos_, &write_area_);
}

// ----------------------------------------------------------------------------
// Set elapsed time (called with each Update)
// ----------------------------------------------------------------------------
void ConsoleEngine::SetTimeElapsed() {
    if (paused_) {
        time_elapsed_ = 1.0*(paused_tick_-start_tick_)/1000;
    } else {
        time_elapsed_ = (1.0*(::GetTickCount()-start_tick_)/1000);
    }
}

// ----------------------------------------------------------------------------
// Return elapsed time
// ----------------------------------------------------------------------------
double ConsoleEngine::time_elapsed() const {
    return time_elapsed_;
}

// ----------------------------------------------------------------------------
// Return delta time
// ----------------------------------------------------------------------------
float ConsoleEngine::GetDeltaTime() {
    // Calculate and return Delta Time
    old_time_ = new_time_;
    new_time_ = time_elapsed();
    return new_time_ - old_time_;
}

// ----------------------------------------------------------------------------
// Place a character (letter) at a specific position (X, Y) with specific style
// ----------------------------------------------------------------------------
void ConsoleEngine::PutPixel(const int X, const int Y, const CHAR letter, const WORD attribute) {
    // Make sure position is within width and height of buffer
    if ( X < 0 || X > width()-1 || Y < 0 || Y > height()-1 ) return;
    // Set character (letter) with style (attribute) at pos (X, Y)
    console_buffer_[X + width() * Y].Char.AsciiChar = letter;
    console_buffer_[X + width() * Y].Attributes = attribute;
}

// ----------------------------------------------------------------------------
// Place a character (letter) at a specific position (index) with specific style
// ----------------------------------------------------------------------------
void ConsoleEngine::PutPixel(const int index, const CHAR letter, const WORD attribute) {
    // Make sure position is within width and height of buffer
    if ( index < 0 || index > (width()*height()) ) return;
    // Set character (letter) with style (attribute) at pos (X, Y)
    console_buffer_[index].Char.AsciiChar = letter;
    console_buffer_[index].Attributes = attribute;
}

// ----------------------------------------------------------------------------
// Get a character and attribute from buffer at a specific position (X, Y)
// ----------------------------------------------------------------------------
void ConsoleEngine::GetPixel(const int X, const int Y, CHAR_INFO *pixel) {
    // Make sure position is within width and height of buffer
    if ( X < 0 || X > width()-1 || Y < 0 || Y > height()-1 ) return;
    // Pixel reference is used to store char and attribute
    pixel->Char.AsciiChar = console_buffer_[X + width() * Y].Char.AsciiChar;
    pixel->Attributes = console_buffer_[X + width() * Y].Attributes;
}

// ----------------------------------------------------------------------------
// Clear console buffer (set all console buffer values to 0 by default)
// ----------------------------------------------------------------------------
void ConsoleEngine::Clear(CHAR letter, WORD color) {
    for (int i = 0; i < width()*height(); i++) {
        console_buffer_[i].Char.AsciiChar = letter;
        console_buffer_[i].Attributes = color;
    }
}

// ----------------------------------------------------------------------------
// Add effect to console engine sequencer
// ----------------------------------------------------------------------------
void ConsoleEngine::AddEffect(Effects::Effect *fx, double time_start, double time_duration) {
    effects_.push_back( (FX){fx, time_start, time_duration} );
}

// ----------------------------------------------------------------------------
// Display or Hide console cursor
// ----------------------------------------------------------------------------
void ConsoleEngine::DisplayCursor(const bool value) {
    // Show blinking cursor again
    CONSOLE_CURSOR_INFO cInfo;
    cInfo.dwSize = 1;
    cInfo.bVisible = value;
    ::SetConsoleCursorInfo(write_handle_, &cInfo);
}
