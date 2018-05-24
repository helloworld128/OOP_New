#include "udptest.h"
#include "ui_udptest.h"
#include "myitem.h"
#include <QHostInfo>
#include <QNetworkInterface>
#include <QListWidgetItem>

UdpTest::UdpTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UdpTest)
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

UdpTest::~UdpTest()
{
    delete ui;
}

//void UdpTest::on_Send_Button_clicked()
//{
//    //    QString localName = QHostInfo::localHostName();
//    //    QHostInfo info = QHostInfo::fromName(localName);
//    //    qDebug() << info.localDomainName();
//    //    qDebug() << info.addresses();
//    //    qDebug() << localName;

//    for (QNetworkInterface interface : QNetworkInterface::allInterfaces())
//    {
//        QList<QNetworkAddressEntry> entryList = interface.addressEntries();

//        //遍历每一个IP地址(每个包含一个IP地址，一个子网掩码和一个广播地址)
//        foreach(QNetworkAddressEntry entry, entryList)
//        {
//            if(entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
//            {
//                //IP地址
//                qDebug() << "IP Address:" << entry.ip().toString();

//                //子网掩码
//                qDebug() << "Netmask:" << entry.netmask().toString();

//                //广播地址
//                qDebug() << "Broadcast:" << entry.broadcast().toString();
//            }
//        }
//    }
//}

void UdpTest::processPendingDatagrams()
{
    QByteArray datagram;
    while (receiver->hasPendingDatagrams()) {
        datagram.resize(int(receiver->pendingDatagramSize()));
        receiver->readDatagram(datagram.data(), datagram.size());
        //        ui->textview->setText(ui->textview->text() + "received messages: " + QString(datagram) + "\n");
        //        ;("received messages: " + QString(datagram));
    }
}

void UdpTest::on_Create_Button_clicked()
{
    QListWidgetItem *aItem = new QListWidgetItem;
    aItem->setSizeHint(QSize(0,80));
    ui->List->addItem(aItem);
    QWidget* item = new MyItem(0, QString("hello"), QString("hell"));
    ui->List->setItemWidget(aItem,item);
}

void UdpTest::on_Join_Button_clicked()
{
    QListWidgetItem* selected = ui->List->currentItem();
    QWidget* _item = ui->List->itemWidget(selected);
    MyItem* item = dynamic_cast<MyItem*>(_item);
    emit PlayReversi();
    close();
}


void UdpTest::on_Leave_Button_clicked(){close();}
