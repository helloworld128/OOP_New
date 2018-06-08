#include "util.h"
#include <QPropertyAnimation>

void setPicture(QLabel* label, const char dir[]){
    QImage img(dir);
    label->setPixmap(QPixmap::fromImage(img));
    label->show();
}

Notice::Notice(QWidget *parent) : QWidget(parent) {
    setGeometry(20,220,540,120);
    label = new QLabel("hello",this);
    label->setStyleSheet("font-size:30px;background-color:white");
    label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    label->setGeometry(0,0,540,120);
    effect = new QGraphicsOpacityEffect(this);
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setParent(this);
    animation->setDuration(2000);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    setGraphicsEffect(effect);
    show();
}

Notice::~Notice(){
    delete label;
    delete effect;
}
