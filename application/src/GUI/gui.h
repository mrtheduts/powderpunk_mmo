#include <QApplication>
#include <QWidget>

class GUI {

    public:
        GUI(int argc, char **argv);

    private:
        QApplication *app;
        QWidget *window_;

        int CreateMainWindow();
        int RunApp();

};

