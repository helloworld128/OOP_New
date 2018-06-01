#include "widget.h"
#include "ui_widget.h"
#include "ai.h"
#include "util.h"
#include <QDebug>
#include <QMouseEvent>
#include <QFileDialog>
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
    ui->Border->hide();
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
        if (game == nullptr) return;
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

//type: 0-reversi, 1-FIR, 2-go
void Widget::createGame(int type, int side, QString name){
    switch(type){
    case 0:
        on_Reversi_Button_clicked();
        break;
    case 1:
        on_FIR_Button_clicked();
        break;
    case 2:
        on_Go_Button_clicked();
        break;
    }
    //game->isOnlineGame = true;
    //connect(game, SIGNAL(put(int,int)), hall, SLOT(sendMove(int,int)));
    //connect(hall, SIGNAL(receiveMove(int,int), game, SLOT(opponentPut(int,int)));
    hall->close();
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
    setPicture(ui->Board, BOARD_1);
    ui->StopOnce_Button->hide();
    ui->GiveUp_Button->hide();
    ui->BCount_LCD->show(); ui->WCount_LCD->show();
    if (hall != nullptr){
        ui->Save_Button->hide(); ui->Load_Button->hide(); ui->Undo_Button->hide();
        ui->bName->setText("payer1");
        ui->wName->setText("plaer2");
        ui->Start_Button->setText("Ready");
        ui->BAI->hide(); ui->BPlayer->hide(); ui->WAI->hide(); ui->WPlayer->hide();
    }

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
    setPicture(ui->Board, BOARD_2);
    setPicture(ui->Border, BORDER);
    ui->StopOnce_Button->hide();
    ui->GiveUp_Button->hide();
    if (hall != nullptr){
        ui->Save_Button->hide(); ui->Load_Button->hide(); ui->Undo_Button->hide();
        ui->bName->setText("payer1");
        ui->wName->setText("plaer2");
        ui->Start_Button->setText("Ready");
        ui->BAI->hide(); ui->BPlayer->hide(); ui->WAI->hide(); ui->WPlayer->hide();
    }
}
void Widget::on_Go_Button_clicked()
{
    game = new Go(this,
                  ui->Board->pos(),
                  ui->CurrentPlayerPict);
    ui->MainMenu->hide();
    ui->GameMenu->show();
    ui->BCount_LCD->hide(); ui->WCount_LCD->hide();
    setPicture(ui->Board, BOARD_2);
    setPicture(ui->Border, BORDER);
    ui->StopOnce_Button->show();
    ui->GiveUp_Button->show();
    if (hall != nullptr){
        ui->Save_Button->hide(); ui->Load_Button->hide(); ui->Undo_Button->hide();
        ui->bName->setText("payer1");
        ui->wName->setText("plaer2");
        ui->Start_Button->setText("Ready");
        ui->BAI->hide(); ui->BPlayer->hide(); ui->WAI->hide(); ui->WPlayer->hide();
    }
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
    ui->Border->hide();
    ui->CurrentPlayerPict->hide();
    delete game;
}

void Widget::on_Save_Button_clicked(){
    QString fileName(tr("game.txt"));
    QString dir = QFileDialog::getExistingDirectory
            (this,tr("Open Directory"),"/home", QFileDialog::ShowDirsOnly
             | QFileDialog::DontResolveSymlinks);
    QDir d;
    QFile file(dir+"/"+fileName);
    file.open(QIODevice::ReadWrite);
    QDataStream stream(&file);
    stream << game-> moveCount;
    for (int t = 0 ; t <= game->moveCount; t++)
    {
        for (int i = 0; i < 9; i++){
            for (int j = 0; j < 9; j++)
                stream << game->previousMove[t][i][j] << "\0";
            stream << "\n";
        }
    }
    file.close();
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
    hall = new WaitingRoom();
    connect(hall, SIGNAL(createGame(int,int,QString)), this, SLOT(createGame(int,int,QString)));
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
