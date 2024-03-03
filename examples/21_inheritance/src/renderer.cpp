// No copyright. Vladislav Aleinik, 2024
#include <renderer.hpp>

//----------------------
// Existance management
//----------------------

Renderer::Renderer(size_t size_x, size_t size_y, const char* window_name) :
    size_x_ (size_x),
    size_y_ (size_y)
{
    // Use current display for new window:
    display_ = XOpenDisplay(nullptr);
    VERIFY_CONTRACT(display_ != nullptr,
        "Renderer(): unable to create display object");

    // Create a new window:
    window_ = XCreateSimpleWindow(
        display_,                     // Specify connection to the X server.
        XDefaultRootWindow(display_), // Do not care about window parent-child relations.
        0, 0,                         // Spawn window in the left upper corner.
        size_x, size_y,               // Set window size
        0,                            // Border width: 0
        0,                            // Border pixel value: 0
        0);                           // Background pixel value: 0 (black)

    // Check the presence of Xdbe extension.
    // NOTE: DBE (Double Buffering Extension) provides an interface
    //       to hardware double buffering through the DRM Linux interface.
    int major_version_return, minor_version_return;
    bool ret = XdbeQueryExtension(display_, &major_version_return, &minor_version_return);
    VERIFY_CONTRACT(ret,
        "Renderer(): XDBE X11 extension is reqired and missing");

    // Initialize graphic context:
    gc_ = XCreateGC(display_, window_, 0, nullptr);

    // Create second framebuffer (back buffer):
    back_buffer_ = XdbeAllocateBackBufferName(display_, window_, 0);

    // Set window name:
    XStoreName(display_, window_, window_name);

    // Create a "close window" action (protocol):
    wm_delete_window_ = XInternAtom(display_, "WM_DELETE_WINDOW", False);

    // Provide Window Manager with a list of actions (protocols)
    // user is willing to perform:
    // - WM_DELETE_WINDOW - action "close the window".
    XSetWMProtocols(display_, window_, &wm_delete_window_, 1);

    // Set the mask of events we are interested in.
    // The only interesting event is KeyPressMask (keyboard key presses).
    XSelectInput(display_, window_, KeyPressMask);

    // Associate the window with display:
    XMapWindow(display_, window_);
}

Renderer::~Renderer()
{
    if (display_ != nullptr)
    {
        // Close connection to the display.
        // NOTE: this eventually deallocates all resources
        //       associated with selected display.
        XCloseDisplay(display_);

        display_ = nullptr;
    }
}

//--------------------------
// Event handling interface
//--------------------------

bool Renderer::events_pending()
{
    return XPending(display_) > 0;
}

XEvent Renderer::next_event()
{
    XEvent event;
    XNextEvent(display_, &event);

    return event;
}

const Atom& Renderer::event_delete_window()
{
    return wm_delete_window_;
}

//----------------------
// Rendering primitives
//----------------------

Renderer& Renderer::clear_screen()
{
    // Use back buffer for rendering:
    XSetForeground(display_, gc_, 0);
    XFillRectangle(display_, back_buffer_, gc_, 0, 0, size_x_, size_y_);

    return *this;
}

Renderer& Renderer::draw_rect(int x, int y, unsigned int sz_x, unsigned int sz_y)
{
    // Use back buffer for rendering:
    XSetForeground(display_, gc_, 0xFF0000);
    XFillRectangle(display_, back_buffer_, gc_, x, y, sz_x, sz_y);

    return *this;
}

Renderer& Renderer::draw_circle(int x, int y, unsigned int r)
{
    // Use back buffer for rendering:
    XSetForeground(display_, gc_, 0x00FF00);
    XFillArc(display_, back_buffer_, gc_, x, y, 2*r, 2*r, 0*64, 360*64);

    return *this;
}

Renderer& Renderer::do_render()
{
    XdbeSwapInfo swap_info;
    swap_info.swap_window = window_;
    swap_info.swap_action = 0;

    // Swap buffers:
    XdbeSwapBuffers(display_, &swap_info, 1);

    return *this;
}
