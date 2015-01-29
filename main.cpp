#include "ConsoleEngine.h"
#include "Composites/Image.h"
#include "Composites/Text.h"
#include "Composites/TextScroller.h"
#include "Effects/Waterfall.h"
#include "Effects/Starfield.h"
#include "Effects/Deform.h"

/*

    TODO: Add split screen effect where 2 or more effect pointers are sent
          and both draw method are called.
          Horizontal, Vertical split screen. (3, 4 splits, etc..)

          Transitions (with or without transparency flag?)

          Mirror composite (kaleidoscope? mirror side then top?)

          Add time when demo should finish? (could be linked to music duration)

*/

int _tmain(int argc, _TCHAR* argv[]) {
    // Create console window object
    ConsoleEngine engine;

    // Init Window - return 1 when failing
    if (!engine.Init(_T("ASCII Demo"), 80, 40)) return 1;

    // ------------------------------------------------------------------------------------------
    // Instantiates effects
    // ------------------------------------------------------------------------------------------
    // Logo
    Effects::Starfield logo_back(&engine);
    Composites::Text txt_logo(&engine, "assets/fonts/chunky", "mrt-prodz", 8, 12);
    // Presents
    Composites::Text txt_presents(&engine, "assets/fonts/chunky", "presents");
    // Title
    Composites::Text txt_title(&engine, "assets/fonts/chunky", "ascii demo", 8, 22);
    // Testing deformations
    Effects::Deform plane(&engine, "assets/sprites/checker2", 20.0f, Effects::Deform::PLANE);
    Effects::Deform tunnel(&engine, "assets/sprites/checker2", 20.0f, Effects::Deform::TUNNEL);
    Effects::Deform flower(&engine, "assets/sprites/checker2", 20.0f, Effects::Deform::FLOWER);
    Effects::Deform spider(&engine, "assets/sprites/checker2", 20.0f, Effects::Deform::SPIDER);
    Effects::Deform plasma(&engine, "assets/sprites/checker2", 20.0f, Effects::Deform::PLASMA);
    // Title
    Composites::Text txt_end(&engine, "assets/fonts/chunky", "to be continued");

    // ------------------------------------------------------------------------------------------
    // Console Engine effects sequencer (FX struct: &effect, timeStart, timeDuration)
    // ------------------------------------------------------------------------------------------
    // Background starfield for logo, presents and title text
    engine.AddEffect( (ConsoleEngine::FX){&logo_back, 0.0, 16.0} );
    engine.AddEffect( (ConsoleEngine::FX){&txt_logo, 0.0, 16.0} );
    engine.AddEffect( (ConsoleEngine::FX){&txt_presents, 4.0, 12.0} );
    engine.AddEffect( (ConsoleEngine::FX){&txt_title, 8.0, 8.0} );
    // Show effects
    engine.AddEffect( (ConsoleEngine::FX){&plane, 16.0, 4.0} );
    engine.AddEffect( (ConsoleEngine::FX){&tunnel, 20.0, 4.0} );
    engine.AddEffect( (ConsoleEngine::FX){&flower, 24.0, 4.0} );
    engine.AddEffect( (ConsoleEngine::FX){&spider, 28.0, 4.0} );
    engine.AddEffect( (ConsoleEngine::FX){&plasma, 32.0, 4.0} );
    // End text
    engine.AddEffect( (ConsoleEngine::FX){&txt_end, 36.0, 8.0} );

    // ------------------------------------------------------------------------------------------
    // Main loop
    // ------------------------------------------------------------------------------------------
    // Game Loop ---------------------------------------
    // http://www.koonsolo.com/news/dewitters-gameloop/
    // Constant Game Speed with Maximum FPS
    const int kTicksPerSecond = 30;
    const int kSkipTicks = 1000 / kTicksPerSecond;
    const int kMaxFrameSkip = 5;
    DWORD next_tick = ::GetTickCount();
    int loops;
    // Main loop
    while (engine.running() && engine.time_elapsed() < 36.0) {
        // Run loop
        loops = 0;
        while (::GetTickCount() > next_tick && loops < kMaxFrameSkip) {
            // Check for key event
            engine.CheckEvent();
            // Update buffer with FX stored in sequencer
            engine.Update();
            // ------------
            next_tick += kSkipTicks;
            loops++;
        }
        // Display buffer
        engine.Render();
    }

    return 0;
}
