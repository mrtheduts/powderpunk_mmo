//#include <Debugtools/debugtools.h>

#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>

#include <iostream>

class GUI {

    public:
        GUI(int argc, char **argv);
        int MakeTestWindow();
        int RunApp();

    private:
        int CreateMainWindow();

        QApplication *qapp;
        QMainWindow *main_window_;
        QTextEdit *main_log;
        QLineEdit *input;
        QVBoxLayout *main_layout;
        QWidget *main_widget;

};

