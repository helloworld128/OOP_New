#include "widget.h"
#include "ui_widget.h"
#include "ai.h"
#include <QDebug>
#include <QMouseEvent>
#include "waitingroom.h"

namespace Ui
{
    class Widget;
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    game = nullptr;
    ui->setupUi(this);
    ui->GameMenu->hide();
    ui->Board->hide();
    ui->MainMenu->hide();
    selectBPlayer.addButton(ui->BPlayer); selectBPlayer.addButton(ui->BAI);
    selectWPlayer.addButton(ui->WPlayer); selectWPlayer.addButton(ui->WAI);
    ui->BPlayer->setChecked(true); ui->WPlayer->setChecked(true);
    setWindowIcon(QIcon(QString("./images/logo.png")));
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
    if(qme->button() == Qt::LeftButton)
    {
        if (game->waiting) return;
        QPoint pos(qme->x(),qme->y());
        if(game != nullptr && game->vTopLeft < pos && pos < game->vBottomRight)
        {
            int x = (pos.x() - game->vTopLeft.x()) / game->gridSize;
            int y = (pos.y() - game->vTopLeft.y()) / game->gridSize;
            game->click(x,y);
        }
    }
}


void Widget::on_Start_Button_clicked()
{
    game->init(ui->BPlayer->isChecked(),ui->WPlayer->isChecked());
}

void Widget::on_Reversi_Button_clicked()
{
    game = new Reversi(this,
                       ui->Board->pos(),
                       ui->BCount_LCD, ui->WCount_LCD,
                       ui->CurrentPlayerPict);
    Ai* ai = new ReversiAi(game);
    QObject::connect(game,SIGNAL(aiPlay()),ai,SLOT(aiPlay()));
    ui->MainMenu->hide();
    ui->Menu->hide();
    ui->GameMenu->show();
    ui->BCount_LCD->show(); ui->WCount_LCD->show();
    QImage* img = new QImage("./images/chessboard.png");
    ui->Board->setPixmap(QPixmap::fromImage(*img));
    ui->Board->show();
    ui->StopOnce_Button->hide();
    ui->GiveUp_Button->hide();

}

void Widget::on_FIR_Button_clicked()
{
    game = new FIR(this,
                   ui->Board->pos(),
                   ui->CurrentPlayerPict);
    Ai* ai = new FIRAi(game);
    QObject::connect(game,SIGNAL(aiPlay()),ai,SLOT(aiPlay()));
    ui->MainMenu->hide();
    ui->GameMenu->show();
    ui->BCount_LCD->hide(); ui->WCount_LCD->hide();
    QImage* img = new QImage("./images/board2.png");
    ui->Board->setPixmap(QPixmap::fromImage(*img));
    ui->Board->show();
    ui->StopOnce_Button->hide();
    ui->GiveUp_Button->hide();

}
void Widget::on_Go_Button_clicked()
{
    game = new Go(this,
                  ui->Board->pos(),
                  ui->CurrentPlayerPict);
    ui->MainMenu->hide();
    ui->GameMenu->show();
    ui->BCount_LCD->hide(); ui->WCount_LCD->hide();
    QImage* img = new QImage("./images/board2.png");
    ui->Board->setPixmap(QPixmap::fromImage(*img));
    ui->Board->show();
    ui->StopOnce_Button->show();
    ui->GiveUp_Button->show();
}

void Widget::on_Undo_Button_clicked()
{
    game->undo();
}

void Widget::on_Menu_Button_clicked()
{
    ui->MainMenu->show();
    ui->GameMenu->hide();
    ui->Board->hide();
    ui->CurrentPlayerPict->hide();
    delete game;
}

void Widget::on_Save_Button_clicked()
{
    QFile saveFile(QString("savedGames/1.txt"));
    if(!saveFile.open(QFile::ReadWrite)) qDebug() << "Failed to open file!";
    QTextStream cout(&saveFile);
    cout << 1;
}

void Widget::on_Load_Button_clicked()
{
    QFile saveFile(QString("savedGames/1.txt"));
    if(!saveFile.open(QFile::ReadWrite)) qDebug() << "Failed to open file!";
    QTextStream cin(&saveFile);
    int data;
    cin >> data;
    qDebug() << data;
}

void Widget::on_Online_Button_clicked()
{
    WaitingRoom* hall = new WaitingRoom();
    connect(hall, SIGNAL(PlayReversi()), this, SLOT(on_Reversi_Button_clicked()));
    hall->exec();
}

void Widget::on_Local_Button_clicked()
{
    ui->Menu->hide();
    ui->MainMenu->show();
}

void Widget::on_Back_Button_clicked()
{
    ui->MainMenu->hide();
    ui->Menu->show();
}

void Widget::on_GiveUp_Button_clicked()
{
    game->showResult();
}

void Widget::on_StopOnce_Button_clicked()
{
    game->nextPlayer();
}
