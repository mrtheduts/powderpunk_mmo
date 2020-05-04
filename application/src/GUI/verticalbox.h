#include <QWidget>
#include<QVBoxLayout>
#include <QVector>
#include <QLineEdit>

class VerticalBox : public QWidget {

    public:
        VerticalBox(QWidget *parent = 0);
        int AddWidget(QWidget *widget);
        QVBoxLayout *vbox_;


    private:
};
