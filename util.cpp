#include "util.h"

void setPicture(QLabel* label, const char dir[]){
    QImage img(dir);
    label->setPixmap(QPixmap::fromImage(img));
    if (label->isHidden()) label->show();
}

Notice::Notice(QWidget *parent) : QWidget(parent) {
    setGeometry(20,220,540,120);
    label = new QLabel("hello",this);
    label->setStyleSheet("font-size:30px;background-color:white");
    label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    label->setGeometry(0,0,540,120);
    effect = new QGraphicsOpacityEffect(this);
    animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(2000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    setGraphicsEffect(effect);
    hide();
}

void Notice::display(const QString &text){
    label->setText(text);
    animation->start();
    raise();
    show();
}

Notice::~Notice(){
    delete label;
    delete animation;
    delete effect;
}
