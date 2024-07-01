#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <iostream>
#include <thread> // for std::this_thread::sleep_for
#include <chrono> // for std::chrono::milliseconds

void sendKey(Display *display, KeySym keysym, bool press)
{
    KeyCode keycode = XKeysymToKeycode(display, keysym);
    if (keycode == 0)
    {
        std::cerr << "No keycode for keysym" << std::endl;
        return;
    }

    // Simulate key press or release
    XTestFakeKeyEvent(display, keycode, press, 0);
    XFlush(display);
}

void handle_top_left_handler()
{
    Display *display = XOpenDisplay(nullptr);
    if (display == nullptr)
    {
        std::cerr << "Unable to open X display" << std::endl;
        return;
    }

    // Simulate Ctrl + Alt + Left Arrow
    sendKey(display, XK_Control_L, true); // Press Ctrl
    sendKey(display, XK_Alt_L, true);     // Press Alt
    sendKey(display, XK_Left, true);      // Press Left Arrow

    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Wait for 200 milliseconds

    sendKey(display, XK_Left, false);      // Release Left Arrow
    sendKey(display, XK_Alt_L, false);     // Release Alt
    sendKey(display, XK_Control_L, false); // Release Ctrl

    // Close the display connection
    XCloseDisplay(display);
}


void handle_top_right_handler()
{
    Display *display = XOpenDisplay(nullptr);
    if (display == nullptr)
    {
        std::cerr << "Unable to open X display" << std::endl;
        return;
    }

    // Simulate Ctrl + Alt + Left Arrow
    sendKey(display, XK_Control_L, true); // Press Ctrl
    sendKey(display, XK_Alt_L, true);     // Press Alt
    sendKey(display, XK_Right, true);      // Press Left Arrow

    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Wait for 200 milliseconds

    sendKey(display, XK_Right, false);      // Release Left Arrow
    sendKey(display, XK_Alt_L, false);     // Release Alt
    sendKey(display, XK_Control_L, false); // Release Ctrl

    // Close the display connection
    XCloseDisplay(display);
}



void handle_bottom_left_handler()
{
    Display *display = XOpenDisplay(nullptr);
    if (display == nullptr)
    {
        std::cerr << "Unable to open X display" << std::endl;
        return;
    }

    // Simulate Windows (Super) key press
    sendKey(display, XK_Super_L, true);  // Press Windows key
    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Wait for 200 milliseconds
    sendKey(display, XK_Super_L, false); // Release Windows key


    // Close the display connection
    XCloseDisplay(display);
}

