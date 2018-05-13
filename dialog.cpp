#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Result"));
}

void Dialog::setText(const QString& str)
{
    ui->label->setText(str);
}

Dialog::~Dialog()
{
    delete ui;
}
