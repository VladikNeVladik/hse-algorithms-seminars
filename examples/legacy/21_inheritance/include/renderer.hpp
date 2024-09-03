// Copyright 2024 Vladislav Aleinik
//=======================================
// A X11-based renderer
//=======================================
#ifndef HEADER_GUARD_FRAMEBUFFER_HPP_INCLUDED
#define HEADER_GUARD_FRAMEBUFFER_HPP_INCLUDED

#include <cstdint>

// X11 window system app development framework
#include <X11/Xlib.h>
// X11 DBE (Double Buffer Extension)
// NOTE: this extension adds access to
//       DRM (Direct Rendering Manager) OS interface.
//       Previous (deprecated) interface lacked
//       double buffering and other common GPU features.
#include <X11/extensions/Xdbe.h>

#include <utils.hpp>

class Renderer
{
public:
    // Existance management:
    Renderer(size_t size_x, size_t size_y, const char* window_name);

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    ~Renderer();

    // Event handling interface:
    bool events_pending();
    XEvent next_event();
    const Atom& event_delete_window();

    // Rendering primitives:
    Renderer& clear_screen();
    Renderer& draw_rect(int x, int y, unsigned int sz_x, unsigned int sz_y);
    Renderer& draw_circle(int x, int y, unsigned int r);
    Renderer& do_render();

private:
    // X11 display used for rendering:
    Display* display_;

    // Newly created X11 window:
    Window window_;

    // Window sizes:
    size_t size_x_;
    size_t size_y_;

    // Graphic context (stores color, line width, etc.):
    GC gc_;

    // Back buffer used in double buffering:
    XdbeBackBuffer back_buffer_;

    // Window exit event:
    Atom wm_delete_window_;
};

#endif // HEADER_GUARD_FRAMEBUFFER_HPP_INCLUDED
