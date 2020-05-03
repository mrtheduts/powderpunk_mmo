#include <GUI/gui.h>

#include <iostream>

// Print only when compiled in debug mode
#ifdef NDEBUG
#define DEBUG(x)
#else
#define DEBUG(x) do {std::cerr << x << std::endl;} while(0)
#endif

int main(int argc, char **argv){

    GUI gui_app(argc, argv);
    DEBUG("GUI object created");

    gui_app.MakeTestWindow();
    gui_app.RunApp();

    return 0;
}
