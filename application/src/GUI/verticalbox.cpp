#include "verticalbox.h"

VerticalBox::VerticalBox(QWidget *parent):QWidget(parent) {

    this->vbox_ = new QVBoxLayout(this);
}

int VerticalBox::AddWidget(QWidget *widget) {

    vbox_->addWidget(widget);

    return 0;
}
