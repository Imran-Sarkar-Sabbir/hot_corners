#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <iostream>
#include <unistd.h>

// void simulateKeyPress(Display* display, KeySym keysym) {
//     KeyCode keycode = XKeysymToKeycode(display, keysym);
//     XTestFakeKeyEvent(display, keycode, True, 0);
//     XTestFakeKeyEvent(display, keycode, False, 0);
//     XFlush(display);
// }

int main() {
    Display* display = XOpenDisplay(NULL);
    if (display == NULL) {
        std::cerr << "Cannot open display" << std::endl;
        return 1;
    }

    int screen = DefaultScreen(display);
    int width = DisplayWidth(display, screen);
    int height = DisplayHeight(display, screen);

    while (true) {
        XEvent event;
        XQueryPointer(display, RootWindow(display, screen),
                      &event.xbutton.root, &event.xbutton.window,
                      &event.xbutton.x_root, &event.xbutton.y_root,
                      &event.xbutton.x, &event.xbutton.y,
                      &event.xbutton.state);

        if (event.xbutton.x == 0 && event.xbutton.y == 0) {
            std::cout << "Top-left corner activated!" << std::endl;
            // simulateKeyPress(display, XK_F1); // Example: simulate pressing F1
        } else if (event.xbutton.x == width - 1 && event.xbutton.y == 0) {
            std::cout << "Top-right corner activated!" << std::endl;
            // simulateKeyPress(display, XK_F2); // Example: simulate pressing F2
        } else if (event.xbutton.x == 0 && event.xbutton.y == height - 1) {
            std::cout << "Bottom-left corner activated!" << std::endl;
            // simulateKeyPress(display, XK_F3); // Example: simulate pressing F3
        } else if (event.xbutton.x == width - 1 && event.xbutton.y == height - 1) {
            std::cout << "Bottom-right corner activated!" << std::endl;
            // simulateKeyPress(display, XK_F4); // Example: simulate pressing F4
        }

        usleep(100000); // Sleep for 100ms
    }

    XCloseDisplay(display);
    return 0;
}
