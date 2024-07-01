#include <iostream>
#include <X11/extensions/Xrandr.h>
#include <X11/Xlib.h>
#include <thread>

void handle_top_left_handler();
void handle_top_right_handler();
void handle_bottom_left_handler();

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

int calculate_mouse_position(int rootX, int rootY, int left, int right, int height)
{
    // mouse top
    if (rootY == 0)
    {
        if (rootX == left)
        {
            return top_left;
        }
        else if (rootX == right)
        {
            return top_right;
        }
    }
    // mouse bottom
    else if (rootY == height)
    {
        if (rootX == left)
        {
            return bottom_left;
        }
        else if (rootX == right)
        {
            return bottom_right;
        }
    }

    return none;
}

void handle_mouse_movement(Display *display, Window &rootWindow, int left, int right, int height)
{
    int rootX, rootY;
    int winX, winY;
    unsigned int mask;
    Window rootReturn, childReturn;

    if (XQueryPointer(display, rootWindow, &rootReturn, &childReturn,
                      &rootX, &rootY, &winX, &winY, &mask) == False)
    {
        std::cerr << "Unable to query pointer" << std::endl;
        return;
    }

    int cursor_status = calculate_mouse_position(rootX, rootY, left, right, height);


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
        }
    }
}

void calculate_values(Display *display, int &right, int &height)
{
    XRRScreenResources *screens = XRRGetScreenResources(display, DefaultRootWindow(display));
    XRRCrtcInfo *info = NULL;
    int i = 0;
    for (i = 0; i < screens->ncrtc; i++)
    {
        info = XRRGetCrtcInfo(display, screens, screens->crtcs[i]);
        right += info->width;
        if (height < info->height)
        {
            height = info->height;
        }
        XRRFreeCrtcInfo(info);
    }

    XRRFreeScreenResources(screens);
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

    int left = 0, right = 0, height = 0;
    calculate_values(display, right, height);
    while (!should_terminate)
    {
        if (!right || !height)
        {
            return 1;
        }


        handle_mouse_movement(display, rootWindow, left, right - 1, height - 1);
    }

    XCloseDisplay(display);
    return 0;
}

int main()
{

    Display *display = XOpenDisplay(nullptr);
    if (display == nullptr)
    {
        std::cerr << "Unable to open X display" << std::endl;
        return 1;
    }
    Window rootWindow = DefaultRootWindow(display);

    int eventBase, errorBase;
    if (!XRRQueryExtension(display, &eventBase, &errorBase))
    {
        std::cerr << "Xrandr extension is not supported" << std::endl;
        XCloseDisplay(display);
        return 1;
    }

    XRRSelectInput(display, rootWindow, RRScreenChangeNotifyMask);

    while (1)
    {

        std::thread thread(main_loop);

        XEvent event;
        XNextEvent(display, &event);
        std::cout << "screen changed" << std::endl;
        should_terminate = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // Wait for 200 milliseconds
        should_terminate = false;
    }
}