// Copyright 2025 Vladislav Aleinik
#ifndef HEADER_GUARD_FRAMEBUFFER_HPP_INCLUDED
#define HEADER_GUARD_FRAMEBUFFER_HPP_INCLUDED

#include <cstdint>

// Linux-специфичная графическая библиотека.
#include <X11/Xlib.h>

// X11 DBE (Double Buffer Extension)
// Дополнительное расширение библиотеки X11, предоставляющее доступ к интерфейсу
// операционной системы DRM (Direct Rendering Manager). Предыдущий (deprecated) интерфейс
// не предоставлял двойной буферизации и прочих возможностей GPU.
#include <X11/extensions/Xdbe.h>

#include <utils.hpp>

//==================//
// Структура данных //
//==================//

struct Renderer
{
    //------------------//
    // Методы структуры //
    //------------------//

    // Управление ресурсами.
    RetCode init(size_t size_x, size_t size_y, const char* window_name);
    void destroy();

    // Обработка событий.
    bool events_pending();
    XEvent next_event();
    const Atom& event_delete_window();

    // Примитивы отрисовки.
    Renderer& clear_screen();
    Renderer& set_pixel(size_t x, size_t y, uint32_t color);
    Renderer& do_render();

    //----------------//
    // Поля структуры //
    //----------------//

    // Дисплей X11.
    Display* display_;
    // Окно X11:
    Window window_;
    // Размеры окна:
    size_t size_x_;
    size_t size_y_;

    // Графический контекст (хранит цвет, ширину линий и т.п.).
    GC gc_;
    // Запасной буфер применяемый при двойной буферизации.
    XdbeBackBuffer back_buffer_;
    // Событие, исполняемое по выходу из окна.
    Atom wm_delete_window_;
};

#endif // HEADER_GUARD_FRAMEBUFFER_HPP_INCLUDED
