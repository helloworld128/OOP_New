#include "widget.h"
#include "ui_widget.h"
#include "ai.h"
#include <QDebug>
#include <QFileDialog>
#include <QTextStream>

namespace Ui
{
class Widget;
}

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    notice = new Notice(this);
    ui->setupUi(this);
    ui->GameMenu->hide();
    ui->Board->hide();
    ui->Border->hide();
    ui->MainMenu->hide();
    ui->OnlineGameMenu->hide();
    ui->Chatting_Button->hide();
    ui->chatText->hide(); ui->chatLabel->hide(); ui->SendText_Button->hide(); ui->textEdit->hide(); ui->Close_Chatting_Button->hide();
    setFixedSize(800, 600);
    selectBPlayer.addButton(ui->BPlayer); selectBPlayer.addButton(ui->BAI);
    selectWPlayer.addButton(ui->WPlayer); selectWPlayer.addButton(ui->WAI);
    ui->BPlayer->setChecked(true); ui->WPlayer->setChecked(true);
    setWindowIcon(QIcon(QString("./images/logo.png")));
    setFixedWidth(800);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::displayNotice(const QString &text){
    notice->display(text);
}

void Widget::setGameUI(int isOnline, int gameType){
    connect(game, SIGNAL(sendNotice(QString)), this, SLOT(displayNotice(QString)));
    ui->MainMenu->hide();
    ui->Menu->hide();
    if (gameType == 0) setPicture(ui->Board, BOARD_1);
    else {
        setPicture(ui->Board, BOARD_2); setPicture(ui->Border, BORDER);
    }
    if (isOnline){
        ui->OnlineGameMenu->show();
        ui->Chatting_Button->show();
        ui->chatText->append("Welcome!");

        if (gameType == 0) {
            ui->BCount_LCD_2->show(); ui->WCount_LCD_2->show();
            ui->BCount_LCD_2->setFixedSize(103, 68);
            ui->WCount_LCD_2->setFixedSize(103, 68);
        }
        else {
            ui->BCount_LCD_2->hide(); ui->WCount_LCD_2->hide();
        }
        if (gameType == 2){
            ui->StopOnce_Button_2->show(); ui->GiveUp_Button_2->show();
        }
        else {
            ui->StopOnce_Button_2->hide(); ui->GiveUp_Button_2->hide();
        }
    }
    else {
        ui->GameMenu->show();
        if (gameType == 0) {
            ui->BCount_LCD->show(); ui->WCount_LCD->show();
        }
        else {
            ui->BCount_LCD->hide(); ui->WCount_LCD->hide();
        }
        if (gameType == 2){
            ui->StopOnce_Button->show(); ui->GiveUp_Button->show();
        }
        else {
            ui->StopOnce_Button->hide(); ui->GiveUp_Button->hide();
        }
    }
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
void Widget::createGame(int type, int side, QString localName, QString otherName){
    switch(type){
    case 0:
        game = new Reversi(this,
                           ui->Board->pos(),
                           ui->BCount_LCD_2, ui->WCount_LCD_2,
                           ui->CurrentPlayerPict);
        setGameUI(1, 0);
        break;
    case 1:
        game = new FIR(this, ui->Board->pos(), ui->CurrentPlayerPict);
        setGameUI(1, 1);
        break;
    case 2:
        game = new Go(this, ui->Board->pos(), ui->CurrentPlayerPict);
        setGameUI(1, 2);
        break;
    }
    ui->bName_2->setText(side == 0? localName : otherName);
    ui->wName_2->setText(side == 1? localName : otherName);
    game->setPlayerType(side);
    game->isOnlineGame = true;
    connect(game, SIGNAL(sendPut(int,int)), hall, SLOT(sendMove(int,int)));
    connect(game, SIGNAL(resetReady()), this, SLOT(resetReady()));
    connect(game, SIGNAL(resetReady()), hall, SLOT(sendGameFinished()));
    connect(hall, SIGNAL(opponentEntered(QString)), this, SLOT(setOpponentName(QString)));
    connect(hall, SIGNAL(opponentPut(int,int)), game, SLOT(opponentPut(int,int)));
    connect(hall, SIGNAL(startGame()), game, SLOT(startGame()));
    connect(hall, SIGNAL(opponentLeft()), game, SLOT(opponentLeft()));
    connect(hall, SIGNAL(opponentChat(QString)), this, SLOT(opponentChat(QString)),Qt::UniqueConnection);
    connect(this, SIGNAL(sendReady()), hall, SLOT(sendReady()));
    connect(this, SIGNAL(sendText(QString)), hall, SLOT(sendText(QString)),Qt::UniqueConnection);
    connect(this, SIGNAL(sendQuit()), hall, SLOT(sendQuit()));
    hall->close();
  //  setFixedWidth(1000);
}

void Widget::resetReady(){
    ui->Ready_Button->setEnabled(true);
}

void Widget::setOpponentName(QString name){
    if (ui->bName_2->text().size() == 0)
        ui->bName_2->setText(name);
    else
        ui->wName_2->setText(name);
}

void Widget::opponentChat(QString text){
    qDebug() << "in Widget opponentChat, text = " << text;
    ui->chatText->append(text);
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
    ai->setParent(game);
    QObject::connect(game,SIGNAL(aiPlay()),ai,SLOT(aiPlay()));
    setGameUI(0, 0);
    notice->display(QString("你好"));
}

void Widget::on_FIR_Button_clicked()
{
    game = new FIR(this,
                   ui->Board->pos(),
                   ui->CurrentPlayerPict);
    Ai* ai = new FIRAi(game);
    ai->setParent(game);
    QObject::connect(game,SIGNAL(aiPlay()),ai,SLOT(aiPlay()));
    setGameUI(0, 1);
}

void Widget::on_Go_Button_clicked()
{
    game = new Go(this,
                  ui->Board->pos(),
                  ui->CurrentPlayerPict);
    setGameUI(0, 2);
}

void Widget::on_Undo_Button_clicked(){
    game->undo();
}

void Widget::on_Menu_Button_clicked()
{
    ui->MainMenu->show();
    ui->GameMenu->hide();
    ui->Board->hide();
    ui->Border->hide();
    ui->CurrentPlayerPict->hide();
    notice->hide();
    delete game;
}

void Widget::on_Save_Button_clicked()
{
    QString fileName(tr("game.txt"));
    QString dir = QFileDialog::getExistingDirectory
            (this,tr("Open Directory"),"/home", QFileDialog::ShowDirsOnly
             | QFileDialog::DontResolveSymlinks);
    QDir d;
    QFile file(dir+"/"+fileName);
    file.open(QIODevice::ReadWrite);
    QTextStream stream(&file);
    stream << int(game->playerType[0]) << ' ' << int(game->playerType[1]) << endl;
    stream << game->moveCount << endl;
    for (int i = 1; i <= game->moveCount; ++i){
        stream << game->previousMovePoint[i].x() << ' ' << game->previousMovePoint[i].y() << ' ';
    }
    stream << game->activePlayer << endl;    
    for (int t = 0 ; t <= game->moveCount; t++)
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                int x = game->previousMove[t][i][j];
                stream << x << "  ";
            }
            stream << endl;
        }
    file.close();
}

void Widget::on_Load_Button_clicked()
{
    QString fileName = QFileDialog::getOpenFileName
            (this, tr("open file"), " ",  tr("Allfile(*.*);;txtfile(*.txt)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite)) return;
    int count, active, ***record;
    record = new int**[10000];
    QTextStream stream(&file);
    int tmp1, tmp2;
    stream >> tmp1 >> tmp2;
    game->playerType[0] = Game::PLAYERTYPE(tmp1); game->playerType[1] = Game::PLAYERTYPE(tmp2);
    stream >> count;
    QPoint* previousMovePoint = new QPoint[count + 1];
    for (int i = 1; i <= count; ++i){
        int x, y;
        stream >> x >> y;
        previousMovePoint[i] = QPoint(x,y);
    }
    stream >> active;
    for (int t = 0 ; t <= count; t++){
        record[t] = new int*[9];
        for (int i = 0; i < 9; i++){
            record[t][i] = new int[9];
            for (int j = 0; j < 9; j++)
                stream >> record[t][i][j];
        }
    }
    game->reStart(count, active, record, previousMovePoint);
    file.close();
}

void Widget::on_Online_Button_clicked()
{
    if (hall == nullptr){
        hall = new WaitingRoom();
        connect(hall, SIGNAL(createGame(int,int,QString,QString)), this, SLOT(createGame(int,int,QString,QString)));
        hall->exec();
    }
    else{
        hall->show();
    }
    //hall->tryToConnect();
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

void Widget::on_Quit_Button_clicked()
{
    int reply = QMessageBox::warning(this, tr("Confirm Quitting"),
                                     tr("Do you really want to quit?"),
                                     QMessageBox::Cancel, QMessageBox::Yes);
    switch (reply){
    case QMessageBox::Yes:
        emit sendQuit();
        ui->Menu->show();
        ui->OnlineGameMenu->hide();
        ui->Board->hide();
        ui->Border->hide();
        ui->CurrentPlayerPict->hide();
        ui->Chatting_Button->hide();
        ui->chatText->clear();
        ui->Ready_Button->setDisabled(false);
        notice->hide();
        setFixedSize(800, 600);
        hall->show();
        setFixedWidth(800);
        delete game;
        break;
    default:
        break;
    }
}

void Widget::on_Ready_Button_clicked()
{
    emit sendReady();
    ui->Ready_Button->setDisabled(true);
}

void Widget::on_Chatting_Button_clicked(){
    ui->chatLabel->show();
    ui->chatText->show();
    ui->SendText_Button->show();
    ui->textEdit->show();
    ui->Close_Chatting_Button->show();
    ui->Chatting_Button->hide();
    setFixedSize(1000, 600);
}

void Widget::on_SendText_Button_clicked(){
    QString ss = ui->textEdit->toPlainText();
    //add player name?
    ui->chatText->append("Me:" + ss);
    ui->textEdit->clear();
    emit sendText(ss);
}

void Widget::on_Close_Chatting_Button_clicked(){
    ui->chatLabel->hide();
    ui->chatText->hide();
    ui->SendText_Button->hide();
    ui->textEdit->hide();
    ui->Close_Chatting_Button->hide();
    ui->Chatting_Button->show();
    setFixedSize(800, 600);
}
