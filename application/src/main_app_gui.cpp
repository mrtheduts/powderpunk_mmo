//#include <Debugtools/debugtools.h>

#include <GUI/gui.h>

#include <iostream>


int main(int argc, char **argv){

    GUI gui_app(argc, argv);
    //DEBUG("GUI object created");

    gui_app.MakeTestWindow();
    gui_app.RunApp();

    return 0;
}
