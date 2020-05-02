#include "gui.h"
#include "splitter.h"

GUI::GUI(int argc, char **argv) {

    QApplication app(argc, argv);
    this->app = &app;

    CreateMainWindow();
    RunApp();
    
}

int GUI::CreateMainWindow() {

    this->window_ = new Splitter();

    window_->resize(350, 300);
    window_->setWindowTitle("Powderpunk Client");
    window_->show();

    return 0;

}

int GUI::RunApp() {

    return app->exec();
}

