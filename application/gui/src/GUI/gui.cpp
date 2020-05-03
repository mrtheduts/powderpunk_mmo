#include "gui.h"

GUI::GUI(int argc, char **argv) {

    std::cout << "Creating qapp" << std::endl;
    qapp = new QApplication(argc, argv);

    CreateMainWindow();
    
}

int GUI::MakeTestWindow() {

    main_log = new QTextEdit();
    main_log->setReadOnly(true);

    input = new QLineEdit();

    main_layout = new QVBoxLayout();
    main_layout->addWidget(main_log);
    main_layout->addWidget(input);

    main_widget = new QWidget(); 
    main_widget->setLayout(main_layout);

    main_window_->setCentralWidget(main_widget);
    return 0;
}

int GUI::CreateMainWindow() {

    main_window_ = new QMainWindow();

    main_window_->resize(1366, 768);
    main_window_->setWindowTitle("Powderpunk Client");

    return 0;

}

int GUI::RunApp() {

    main_window_->show();
    return qapp->exec();
}

