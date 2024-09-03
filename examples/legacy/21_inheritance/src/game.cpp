// Copyright 2024 Vladislav Aleinik

#include <cstdlib>
#include <ctime>
#include <random>

#include <unistd.h>

#include <renderer.hpp>
#include <objects.hpp>

// Window resolution:
const size_t WIDTH  = 1200;
const size_t HEIGHT = 800;

// Rendering parameters:
const unsigned FPS = 30U;
const useconds_t FRAME_US = 1000*1000/FPS;

constexpr double PI = std::acos(-1);

int main()
{
    Renderer renderer{WIDTH, HEIGHT, "Inheritance example"};

    // Initialize random number generator:
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    // Initialize scene:
    std::vector<RenderedObject*> objects{};

    for (size_t i = 0U; i < 20U; ++i)
    {
        double x = 100.0 + dist(gen) * (WIDTH  - 200);
        double y = 100.0 + dist(gen) * (HEIGHT - 200);

        double angle = dist(gen) * 2.0*PI;
        double mod   = dist(gen) * 40.0 + 10.0;

        double v_x = mod * std::cos(angle);
        double v_y = mod * std::sin(angle);

        if (rand() % 2)
        {
            Square* square = new Square(x, y, 50, 50, v_x, v_y, 10.0);

            objects.push_back(static_cast<RenderedObject*>(square));
        }
        else
        {
            Round* round = new Round(x, y, 50, v_x, v_y, 2.0);

            objects.push_back(static_cast<RenderedObject*>(round));
        }
    }

    bool quit = false;
    while (!quit)
    {
        // Start measuring frame rendering time:
        // NOTE: we are aiming at constant framerate.
        clock_t start = clock();

        // User-controlled acceleration:
        double f_x = 0.0;
        double f_y = 0.0;

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
                        case 'w':
                        {
                            f_y -= 500.0;
                            break;
                        }
                        case 'a':
                        {
                            f_x -= 500.0;
                            break;
                        }
                        case 's':
                        {
                            f_y += 500.0;
                            break;
                        }
                        case 'd':
                        {
                            f_x += 500.0;
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

        for (RenderedObject* obj : objects)
        {
            obj->render(renderer);
        }

        renderer.do_render();

        // Calculate physics for the next frame:
        for (auto& obj : objects)
        {
            obj->apply_force(f_x, f_y, 1.0 / FPS);
            obj->move(1.0 / FPS);
            obj->dissipate(0.5, 1.0 / FPS);
            obj->wall_bounce(0, 0, WIDTH, HEIGHT);
        }

        // Start measuring frame rendering time:
        // NOTE: we are aiming at constant framerate.
        clock_t end = clock();

        useconds_t delta_us = (end - start) * 1000000U / CLOCKS_PER_SEC;
        if (delta_us > FRAME_US)
        {
            delta_us = FRAME_US;
        }

        // Do nothing until the next frame is here:
        usleep(FRAME_US - delta_us);
    }

    // Deallocate all objects:
    for (RenderedObject* obj : objects)
    {
        delete obj;
    }

    return EXIT_SUCCESS;
}
