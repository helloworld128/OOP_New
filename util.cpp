#include "util.h"

void setPicture(QLabel* label, const char dir[]){
    QImage img(dir);
    label->setPixmap(QPixmap::fromImage(img));
    label->show();
}
