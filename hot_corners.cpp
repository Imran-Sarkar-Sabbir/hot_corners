#include <iostream>
#include <X11/extensions/Xrandr.h>
#include <X11/Xlib.h>
#include <thread>

void handle_top_left_handler();
void handle_top_right_handler();
void handle_bottom_left_handler();
void handle_bottom_right_handler();

bool isHotCornerEnabled = false;
bool should_terminate = false;

enum Cornor_point
{
    none,
    top_left,
    top_right,
    bottom_left,
    bottom_right
};

int calculate_mouse_position(Display *display, int &screen, int height, int width)
{

    XEvent event;
    XQueryPointer(display, RootWindow(display, screen),
                  &event.xbutton.root, &event.xbutton.window,
                  &event.xbutton.x_root, &event.xbutton.y_root,
                  &event.xbutton.x, &event.xbutton.y,
                  &event.xbutton.state);

    if (event.xbutton.x == 0 && event.xbutton.y == 0)
    {
        return top_left;
    }
    else if (event.xbutton.x == width - 1 && event.xbutton.y == 0)
    {
        return top_right;
    }
    else if (event.xbutton.x == 0 && event.xbutton.y == height - 1)
    {
        return bottom_left;
    }
    else if (event.xbutton.x == width - 1 && event.xbutton.y == height - 1)
    {
        return bottom_right;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Wait for 100 milliseconds
    return none;
}

void handle_mouse_movement(Display *display, int &screen, int &height, int &width)
{

    int cursor_status = calculate_mouse_position(display, screen, height, width);

    if (cursor_status == none)
    {
        isHotCornerEnabled = false;
    }
    else
    {
        if (isHotCornerEnabled)
            return;
        isHotCornerEnabled = true;

        switch (cursor_status)
        {
        case top_left:
            handle_top_left_handler();
            break;
        case top_right:
            handle_top_right_handler();
            break;

        case bottom_left:
            handle_bottom_left_handler();
            break;

        case bottom_right:
            handle_bottom_right_handler();
            break;
        }
    }
}

int main_loop()
{

    Display *display = XOpenDisplay(nullptr);
    if (display == nullptr)
    {
        std::cerr << "Unable to open X display" << std::endl;
        return 1;
    }
    Window rootWindow = DefaultRootWindow(display);
    int screen = DefaultScreen(display);
    int width = DisplayWidth(display, screen);
    int height = DisplayHeight(display, screen);

    while (!should_terminate)
    {
        handle_mouse_movement(display, screen, height, width);
    }

    XCloseDisplay(display);
    return 0;
}

int main()
{

    // Display *display = XOpenDisplay(nullptr);
    // if (display == nullptr)
    // {
    //     std::cerr << "Unable to open X display" << std::endl;
    //     return 1;
    // }
    // Window rootWindow = DefaultRootWindow(display);

    // int eventBase, errorBase;
    // if (!XRRQueryExtension(display, &eventBase, &errorBase))
    // {
    //     std::cerr << "Xrandr extension is not supported" << std::endl;
    //     XCloseDisplay(display);
    //     return 1;
    // }

    // XRRSelectInput(display, rootWindow, RRScreenChangeNotifyMask);

    // while (1)
    // {

        std::thread thread(main_loop);
        thread.join();

    //     XEvent event;
    //     XNextEvent(display, &event);
    //     std::cout << "screen changed" << std::endl;
    //     should_terminate = true;
    //     std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Wait for 200 milliseconds
    //     should_terminate = false;
    // }
}