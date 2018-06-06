#include "waitingroom.h"
#include "myitem.h"
#include "ui_waitingroom.h"
#include <QHostInfo>
#include <QNetworkInterface>
#include <QListWidgetItem>

WaitingRoom::WaitingRoom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitingRoom)
{
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress::LocalHost,23333);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    ui->setupUi(this);
    ui->ChooseGame->hide();
    games.addButton(ui->rButton); games.addButton(ui->fButton); games.addButton(ui->gButton);
    role.addButton(ui->black); games.addButton(ui->white);
    ui->rButton->setChecked(true); ui->black->setChecked(true);
    requestCurrentGames();

    QListWidgetItem *aItem = new QListWidgetItem;
    aItem->setSizeHint(QSize(0,80));
    ui->List->addItem(aItem);
    ui->List->setItemWidget(aItem,new MyItem(0, QString("hello"), QString("")));
}

void WaitingRoom::requestCurrentGames(){
    socket->write("g");
}

WaitingRoom::~WaitingRoom()
{
    delete ui;
}

void WaitingRoom::readData()
{
    char type;
    auto _content = socket->readAll();//.data();
    auto content = _content.data();
    sscanf(content, "%c", &type);
    switch (type){
    //gameList received
    case 'g':
        //int n;
        int next;
        sscanf(content + 1, "%d", &next);
        qDebug() << next;
        break;
        //opponent ready
    case 'r':
        emit opponentReady();
        break;
        //opponent put
    case 'p':
        char x, y;
        sscanf(content + 1, "%c%c", &x, &y);
        emit opponentPut(x - '0', y - '0');
        break;
        //opponent entered
    case 'e':
        char name[10];
        sscanf(content + 1, "%s", name);
        emit opponentEntered(name);
        break;
        //opponent left
    case 'l':
        emit opponentLeft();
        break;
        //TODO: opponent surrendered? other functions?
    default:
        qDebug() << "unknown command received!";
        break;
    }
}

void WaitingRoom::sendMove(int x, int y){
    QByteArray tmp;
    tmp.append('p').append(x + '0').append(y + '0');
    socket->write(tmp);
}

void WaitingRoom::sendReady(){
    socket->write("r");
}

void WaitingRoom::on_Create_Button_clicked()
{
    ui->ChooseGame->show();
    ui->Buttons->hide();
    ui->Display->hide();
}

void WaitingRoom::on_Join_Button_clicked()
{
    QListWidgetItem* selected = ui->List->currentItem();
    if (selected == nullptr) return;
    QWidget* _item = ui->List->itemWidget(selected);
    MyItem* item = dynamic_cast<MyItem*>(_item);
    int tmpSide = 0;
    QString tmpName;
    if (item->nameb.size() == 0){
        tmpSide = 1;
        tmpName = item->namew;
    }
    if (item->namew.size() == 0){
        tmpSide = 0;
        tmpName = item->nameb;
    }
    //found a vacant seat
    if (tmpName.size()){
        QByteArray tmp;
        char _uid[5];
        itoa(item->uid, _uid, 10);
        tmp.append('j').append(_uid);
        socket->write(tmp);
        createGame(item->type, tmpSide, playerName, tmpName);
        hide();
    }
}

void WaitingRoom::on_Leave_Button_clicked(){close();}

void WaitingRoom::on_Spectate_Button_clicked()
{
    socket->write(QByteArray::fromStdString("hello!!!"));
}

void WaitingRoom::on_Cancel_Button_clicked()
{
    ui->ChooseGame->hide();
    ui->Buttons->show();
    ui->Display->show();
}

void WaitingRoom::on_OK_Button_clicked()
{
    int tmpType;
    int tmpSide;
    if (ui->rButton->isChecked()) tmpType = 0;
    else if (ui->fButton->isChecked()) tmpType = 1;
    else tmpType = 2;
    tmpSide = ui->white->isChecked();

    emit createGame(tmpType, tmpSide, playerName);
}

void WaitingRoom::on_lineEdit_editingFinished()
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

void WaitingRoom::on_Refresh_Button_clicked()
{
    socket->write("g");
}
