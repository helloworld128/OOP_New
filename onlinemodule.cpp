#include "onlinemodule.h"
#include "myitem.h"
#include "util.h"
#include "ui_onlinemodule.h"
#include <QHostInfo>
#include <QNetworkInterface>
#include <QListWidgetItem>
#include <QTime>

OnlineModule::OnlineModule(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OnlineModule)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    socket->connectToHost(QHostAddress(SERVER_IP),23333);
    ui->setupUi(this);
    ui->ChooseGame->hide();
    ui->Display->hide();
    ui->Buttons->hide();
    games.addButton(ui->rButton); games.addButton(ui->fButton); games.addButton(ui->gButton);
    role.addButton(ui->black); role.addButton(ui->white);
    ui->rButton->setChecked(true); ui->black->setChecked(true);

    //generate a random player name
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int rnd = qrand() % 9000 + 1000;
    playerName = "Guest#" + QString::number(rnd);
    ui->lineEdit->setText(playerName);

//    ui->IP->hide();
//    ui->label->hide();
}

void OnlineModule::connected(){
    b_connected = true;
    QTime t; t.start();
    while (t.elapsed() < 1000) QCoreApplication::processEvents();
    ui->Display->show();
    ui->Buttons->show();
    ui->Connecting->hide();
    requestCurrentGames();
}

void OnlineModule::requestCurrentGames(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << QChar('g');
    socket->write(ba);
}

OnlineModule::~OnlineModule()
{
    delete ui;
}

void OnlineModule::addGame(int type, QString nameb, QString namew, int uid){
    QListWidgetItem *aItem = new QListWidgetItem;
    aItem->setSizeHint(QSize(0,80));
    ui->List->addItem(aItem);
    ui->List->setItemWidget(aItem,new MyItem(type, nameb, namew, uid));
}

void OnlineModule::sendGameFinished(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << QChar('f');
    qDebug() << "f sent";
    socket->waitForBytesWritten();
    socket->write(ba);
}

//used characters: g(request game list), p(put), q(quit), e(enter), s(start), t(chat), w(spectate)
void OnlineModule::readData(){
    QByteArray ba = socket->readAll();
    QDataStream in(&ba, QIODevice::ReadOnly);
    QChar t; in >> t;
    //convert QChar to char
    char type = t.toLatin1();
    switch (type){
    case 'g':
    {
        ui->List->clear();
        int n;
        in >> n;
        int t, id; QString b, w;
        for (int i = 0; i < n; i++){
            in >> t >> id >> b >> w;
            addGame(t, b, w, id);
        }
        break;
    }
    case 'p':
    {
        int x, y;
        in >> x >> y;
        emit opponentPut(x, y);
        //if (isWatching) emit watchPut(x, y);
        break;
    }
    case 'q':
        emit opponentLeft(isWatching);
        break;
    case 'e':
    {
        QString name;
        in >> name;
        emit opponentEntered(name);
        break;
    }
    case 's':
        emit startGame();
        break;
    case 'u':
    {
        emit opponentGiveUp();
        break;
    }
    case 'o':
    {
        emit opponentStopOnce();
        break;
    }
    case 't':
    {
        QString name, text;
        in >> name >> text;
        emit opponentChat(name + ":" + text);
        break;
    }
    case 'z':
    {
        if (isWatching)
        {
            int **tmpBoard;
            int currentPlayer;
            tmpBoard = new int*[9];
            for (int i = 0; i < 9; ++i){
                tmpBoard[i] = new int[9];
            }
            for (int i = 0; i < 9; ++i){
                for (int j = 0; j < 9; ++j){
                    in >> tmpBoard[i][j];
                }
            }
            in >> currentPlayer;
            emit sendBoard(tmpBoard, currentPlayer);
            for (int i = 0; i < 9; ++i)
                delete[] tmpBoard[i];
            delete tmpBoard;
        }
        else emit requestBoard();
        break;
    }
    default:
        qDebug() << "unknown command!";
        break;
    }
}

void OnlineModule::sendMove(int x, int y){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << QChar('p') << x << y;
    socket->write(ba);
}

void OnlineModule::sendReady(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << QChar('r');
    socket->write(ba);
}

void OnlineModule::sendQuit(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << QChar('q');
    socket->write(ba);
}

void OnlineModule::sendGiveUp(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << QChar('u');
    socket->write(ba);
}

void OnlineModule::sendStopOnce(){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << QChar('o');
    socket->write(ba);
}

void OnlineModule::sendText(QString text){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << QChar('t') << playerName << text;
    socket->write(ba);
}

void OnlineModule::receiveBoard(int **board, int currentPlayer){
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << QChar('z');
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++){
            out << board[i][j];
        }
    out << currentPlayer;
    socket->write(ba);
}

void OnlineModule::on_Create_Button_clicked()
{
    ui->ChooseGame->show();
    ui->Buttons->hide();
    ui->Display->hide();
}

void OnlineModule::on_Join_Button_clicked()
{
    QListWidgetItem* selected = ui->List->currentItem();
    if (selected == nullptr) return;
    QWidget* _item = ui->List->itemWidget(selected);
    MyItem* item = dynamic_cast<MyItem*>(_item);
    int tmpSide = 0;
    QString tmpName;
    if (item->nameb.size() == 0){
        tmpSide = 0;
        tmpName = item->namew;
    }
    if (item->namew.size() == 0){
        tmpSide = 1;
        tmpName = item->nameb;
    }
    //found a vacant seat
    if (tmpName.size()){
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        out << QChar('j') << item->uid << tmpSide << playerName;
        socket->write(ba);
        emit createGame(item->type, tmpSide, playerName, tmpName);
        hide();
    }
}

void OnlineModule::on_Leave_Button_clicked(){close();}

void OnlineModule::on_Spectate_Button_clicked()
{
    QListWidgetItem* selected = ui->List->currentItem();
    if (selected == nullptr) return;
    QWidget* _item = ui->List->itemWidget(selected);
    MyItem* item = dynamic_cast<MyItem*>(_item);
    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << QChar('w') << item->uid;
    socket->write(ba);
    isWatching = true;

    //the value of 'side' for spectators is 2.
    emit spectate(item->type, 2, item->nameb, item->namew);
    hide();
}

void OnlineModule::on_Cancel_Button_clicked()
{
    ui->ChooseGame->hide();
    ui->Buttons->show();
    ui->Display->show();
}

void OnlineModule::on_OK_Button_clicked()
{
    int tmpType;
    int tmpSide;
    if (ui->rButton->isChecked()) tmpType = 0;
    else if (ui->fButton->isChecked()) tmpType = 1;
    else tmpType = 2;
    tmpSide = ui->white->isChecked();

    QByteArray ba;
    QDataStream out(&ba, QIODevice::WriteOnly);
    out << QChar('c') << tmpType << tmpSide << playerName;
    socket->write(ba);

    emit createGame(tmpType, tmpSide, playerName);
}

void OnlineModule::on_lineEdit_editingFinished()
{
    QString tmp = ui->lineEdit->text();
    if (tmp.size() > 10){
        qDebug() << "max playerName length == 10!";
        ui->lineEdit->setText(playerName);
    }
    else{
        playerName = tmp;
    }
}

void OnlineModule::on_Refresh_Button_clicked()
{
    requestCurrentGames();
}

void OnlineModule::on_IP_editingFinished()
{
    static QString previous = "";
    if (previous == ui->IP->text()) return;
    qDebug() << ui->IP->text();
    previous = ui->IP->text();
    socket->disconnectFromHost();
    socket->connectToHost(QHostAddress(ui->IP->text()), 23333);
}
