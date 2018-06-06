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
    effect = new QGraphicsOpacityEffect(this);
    effect->setOpacity(0.5);
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    animation->setDuration(2000);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
    setGraphicsEffect(effect);
    show();
}
