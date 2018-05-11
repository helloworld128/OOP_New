#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include <QMouseEvent>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->Board->hide();
    ui->BCount_Label->hide(); ui->WCount_Label->hide(); ui->BCount_LCD->hide(); ui->WCount_LCD->hide();
    ui->Start_Button->hide(); ui->Undo_Button->hide(); ui->Menu_Button->hide();
}

Widget::~Widget()
{
    delete ui;
}

bool operator <(const QPoint& left,const QPoint& right)
{
    return left.x() < right.x() && left.y() < right.y();
}

void Widget::mousePressEvent(QMouseEvent *qme)
{
    if(qme->button() & Qt::LeftButton)
    {
        QPoint presspos(qme->x(),qme->y());
        qDebug() << presspos;
        if(game && game->vTopLeft < presspos && presspos < game->vBottomRight)
        {
            game->putChess(presspos);
        }
    }
}


void Widget::on_Start_Button_clicked()
{
    game->init();
}

void Widget::hideHomeScreenUI()
{
    ui->Title_Label->hide();
    ui->Reversi_Button->hide();
    ui->FIR_Button->hide();
    ui->Go_Button->hide();
}

void Widget::showHomeScreenUI()
{
    ui->Title_Label->show();
    ui->Reversi_Button->show();
    ui->FIR_Button->show();
    ui->Go_Button->show();
}

void Widget::on_Reversi_Button_clicked()
{
    game = new Reversi(this);
    Reversi* ptr = dynamic_cast<Reversi*>(game);
    ptr->black = ui->BCount_LCD;
    ptr->white = ui->WCount_LCD;
    hideHomeScreenUI();
    ui->Board->show();
    ui->BCount_Label->show(); ui->WCount_Label->show(); ui->BCount_LCD->show(); ui->WCount_LCD->show();
    ui->Start_Button->show(); ui->Undo_Button->show(); ui->Menu_Button->show();
}

void Widget::on_FIR_Button_clicked()
{
//    game = new FIR(this);
}
void Widget::on_Go_Button_clicked()
{

}

void Widget::on_Undo_Button_clicked()
{
    game->undo();
}

void Widget::on_Menu_Button_clicked()
{
    showHomeScreenUI();
    ui->Board->hide();
    ui->BCount_Label->hide(); ui->WCount_Label->hide(); ui->BCount_LCD->hide(); ui->WCount_LCD->hide();
    ui->Start_Button->hide(); ui->Undo_Button->hide(); ui->Menu_Button->hide();
    ui->BCount_LCD->display(0); ui->WCount_LCD->display(0);
    delete game;
}
