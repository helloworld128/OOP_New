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
    socket->connectToHost(QHostAddress("183.172.107.254"),23333);
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
    ui->List->setItemWidget(aItem,new MyItem(0, QString("hello"), QString("hell")));
}

void WaitingRoom::requestCurrentGames(){
    socket->write(QByteArray::fromStdString("r"));
}

WaitingRoom::~WaitingRoom()
{
    delete ui;
}

void WaitingRoom::readData()
{

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
    QWidget* _item = ui->List->itemWidget(selected);
    MyItem* item = dynamic_cast<MyItem*>(_item);
    emit PlayReversi();
    hide();
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

}

void WaitingRoom::on_lineEdit_editingFinished()
{
    qDebug() << ui->lineEdit->text();
}

void WaitingRoom::on_Refresh_Button_clicked()
{

}
