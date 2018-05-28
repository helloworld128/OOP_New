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
    sender = new QUdpSocket(this);
    receiver = new QUdpSocket(this);
    receiver->bind(QHostAddress::AnyIPv4,23333,QUdpSocket::ShareAddress);
    connect(receiver, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));
    ui->setupUi(this);
    QListWidgetItem *aItem = new QListWidgetItem;
    aItem->setSizeHint(QSize(0,80));
    ui->List->addItem(aItem);
    ui->List->setItemWidget(aItem,new MyItem(0, QString("hello"), QString("hell")));
}

WaitingRoom::~WaitingRoom()
{
    delete ui;
}

void WaitingRoom::processPendingDatagrams()
{
    QByteArray datagram;
    while (receiver->hasPendingDatagrams()) {
        datagram.resize(int(receiver->pendingDatagramSize()));
        receiver->readDatagram(datagram.data(), datagram.size());
        //        ui->textview->setText(ui->textview->text() + "received messages: " + QString(datagram) + "\n");
        //        ;("received messages: " + QString(datagram));
    }
}

void WaitingRoom::on_Create_Button_clicked()
{
    QListWidgetItem *aItem = new QListWidgetItem;
    aItem->setSizeHint(QSize(0,80));
    ui->List->addItem(aItem);
    QWidget* item = new MyItem(0, QString("hello"), QString("hell"));
    ui->List->setItemWidget(aItem,item);
}

void WaitingRoom::on_Join_Button_clicked()
{
    QListWidgetItem* selected = ui->List->currentItem();
    QWidget* _item = ui->List->itemWidget(selected);
    MyItem* item = dynamic_cast<MyItem*>(_item);
    emit PlayReversi();
    close();
}

void WaitingRoom::on_Leave_Button_clicked(){close();}



void WaitingRoom::on_Spectate_Button_clicked()
{
}