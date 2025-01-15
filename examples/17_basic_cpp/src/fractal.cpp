// Copyright 2024 Vladislav Aleinik

#include <cstdlib>
#include <ctime>
#include <random>

#include <unistd.h>

#include <renderer.hpp>

// Разрешение окна.
const int WIDTH  = 1200;
const int HEIGHT = 800;

// Параметры отрисовки.
const unsigned FPS = 5U;
const useconds_t FRAME_US = 1000*1000/FPS;
const size_t MAX_ITERATIONS = 4*4*4;

int main()
{
    // Создаём и инициализируем рендерер.
    Renderer renderer;
    renderer.init(WIDTH, HEIGHT, "Fractal");

    // Текущие координаты и размер пикселя.
    double screen_x = 0.0;
    double screen_y = 0.0;
    double zoom = 0.001;

    bool quit = false;
    while (!quit)
    {
        // Время начала вычисления следующего шага рассчёта фрактала.
        clock_t start = clock();

        // Скорости изменения всех параметров.
        double v_x   = 0.0;
        double v_y   = 0.0;
        double scale = 1.0;

        // Обрабатываем команды пользователя.
        while (renderer.events_pending())
        {
            XEvent event = renderer.next_event();
            switch (event.type) {
                case KeyPress:
                {
                    switch (XLookupKeysym(&event.xkey, 0))
                    {
                        case 'q': { quit = 1U;     break; }
                        case 'w': { v_y -= 2*zoom; break; }
                        case 'a': { v_x -= 2*zoom; break; }
                        case 's': { v_y += 2*zoom; break; }
                        case 'd': { v_x += 2*zoom; break; }
                        case 'p': { scale /= 1.02; break; }
                        case 'o': { scale *= 1.02; break; }
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

        // Обновляем координаты и размер пикселя.
        screen_x += v_x;
        screen_y += v_y;
        zoom *= scale;

        // Вычисляем значения цветов фрактала.
        renderer.clear_screen();

        for (int i_x = 0; i_x < WIDTH; ++i_x)
        {
            for (int i_y = 0; i_y < HEIGHT; ++i_y)
            {
                double x0 = screen_x + (i_x -  WIDTH/2) * zoom;
                double y0 = screen_y + (i_y - HEIGHT/2) * zoom;

                double x = 0.0;
                double y = 0.0;
                double x2 = 0.0;
                double y2 = 0.0;

                // Вычисляем номер цвета в палитре.
                size_t iteration = 0;
                while (x2 + y2 <= 4 && iteration < MAX_ITERATIONS)
                {
                    y  = 2 * x * y + y0;
                    x  = x2 - y2 + x0;
                    x2 = x * x;
                    y2 = y * y;
                    iteration++;
                }

                uint32_t color =
                    ((iteration & 0x30) << 18) |
                    ((iteration & 0x0C) << 12) |
                    ((iteration & 0x03) <<  6);

                // Выставляем пиксль нужного цвета.
                renderer.set_pixel(i_x, i_y, color);
            }
        }

        // Отображаем текущий кадр.
        renderer.do_render();

        // Время конца вычисления следующего шага рассчёта фрактала.
        clock_t end = clock();

        useconds_t delta_us = (end - start) * 1000000U / CLOCKS_PER_SEC;
        if (delta_us > FRAME_US)
        {
            delta_us = FRAME_US;
        }

        // Ожидаем начала следующего кадра.
        usleep(FRAME_US - delta_us);
    }

    // Удаляем рендерер.
    renderer.destroy();

    return EXIT_SUCCESS;
}
