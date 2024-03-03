// No copyright. Vladislav Aleinik, 2024

#include <cstdlib>
#include <ctime>

#include <unistd.h>

#include <renderer.hpp>

// Window resolution:
const size_t WIDTH  = 1200;
const size_t HEIGHT = 800;

// Rendering parameters:
const unsigned FPS = 30U;
const useconds_t FRAME_US = 1000*1000/FPS;

int main()
{
    Renderer renderer{WIDTH, HEIGHT, "Inheritance example"};

    bool quit = false;
    while (!quit)
    {
        // Start measuring frame rendering time:
        // NOTE: we are aiming at constant framerate.
        clock_t start = clock();

        // Handle incoming events:
        while (renderer.events_pending())
        {
            XEvent event = renderer.next_event();
            switch (event.type) {
                case KeyPress:
                {
                    switch (XLookupKeysym(&event.xkey, 0))
                    {
                        case 'q':
                        {
                            quit = 1U;
                            break;
                        }
                        default: {}
                    }
                    break;
                }
                case ClientMessage:
                {
                    if ((Atom) event.xclient.data.l[0] == renderer.event_delete_window())
                    {
                        quit = 1;
                    }
                    break;
                }
            }
        }

        // Render a frame:
        renderer.clear_screen();

        renderer.draw_rect(100, 100, 200, 200);
        renderer.draw_circle(400, 400, 100);

        renderer.do_render();

        // Calculate physics for the next frame:
        // ToDo

        // Start measuring frame rendering time:
        // NOTE: we are aiming at constant framerate.
        clock_t end = clock();

        useconds_t delta_us = (end - start) * 1000000U / CLOCKS_PER_SEC;
        if (delta_us > FRAME_US)
        {
            delta_us = FRAME_US;
        }

        // Do nothing until next frame is here:
        usleep(FRAME_US - delta_us);
    }

    return EXIT_SUCCESS;
}
