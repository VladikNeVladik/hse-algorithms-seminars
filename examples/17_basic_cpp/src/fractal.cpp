// Copyright 2024 Vladislav Aleinik

#include <cstdlib>
#include <ctime>
#include <random>
#include <thread>

#include <unistd.h>

#include <renderer.hpp>

// Разрешение окна.
const int WIDTH  = 1200;
const int HEIGHT = 800;

// Параметры отрисовки.
const unsigned FPS = 5U;
const useconds_t FRAME_US = 1000*1000/FPS;
const size_t MAX_ITERATIONS = 4*4*4;

// Функция вычисления цветов фрактала.
void compute_fractal_colors(
    uint32_t* colors, int thread_i, int num_threads,
    double screen_x, double screen_y, double zoom)
{
    for (int i_x = thread_i * WIDTH/num_threads;
        i_x < (thread_i+1) * WIDTH/num_threads; i_x += 1)
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

            // Выставляем пиксель нужного цвета.
            colors[i_x * HEIGHT + i_y] = color;
        }
    }
}

int main()
{
    // Создаём и инициализируем рендерер.
    Renderer renderer;
    renderer.init(WIDTH, HEIGHT, "Fractal");

    // Аллоцируем массив цветов в динамической памяти.
    uint32_t* colors = new uint32_t[WIDTH * HEIGHT];

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

        // Количество потоков для вычисления карты пикселей.
        int num_threads = std::thread::hardware_concurrency();

        // Массив идентификаторов потоков.
        std::thread threads[num_threads - 1];

        // Создаём потоки для вычисления частей карты пикселей.
        for (int thread_i = 1; thread_i < num_threads; ++thread_i)
        {
            threads[thread_i - 1] = std::thread(
                compute_fractal_colors, colors, thread_i, num_threads,
                screen_x, screen_y, zoom);
        }

        // Производим вычисление для нулевого потока.
        compute_fractal_colors(colors, 0, num_threads, screen_x, screen_y, zoom);

        // Ожидаем завершения всех остальных потоков.
        for (auto& thread : threads)
        {
            thread.join();
        }

        // Очищаем экран перед отрисовкой.
        renderer.clear_screen();

        // Вычисляем значения цветов фрактала.
        for (int i_x = 0; i_x < WIDTH; ++i_x)
        {
            for (int i_y = 0; i_y < HEIGHT; ++i_y)
            {
                // Выставляем пиксль нужного цвета.
                renderer.set_pixel(i_x, i_y, colors[i_x * HEIGHT + i_y]);
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

    // Освобождаем массив цветов.
    delete[] colors;

    // Удаляем рендерер.
    renderer.destroy();

    return EXIT_SUCCESS;
}
