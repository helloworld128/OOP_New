#include "udptest.h"
#include "ui_udptest.h"
#include <QHostInfo>
#include <QNetworkInterface>

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
}

UdpTest::~UdpTest()
{
    delete ui;
}

void UdpTest::on_Send_Button_clicked()
{
    //    QString localName = QHostInfo::localHostName();
    //    QHostInfo info = QHostInfo::fromName(localName);
    //    qDebug() << info.localDomainName();
    //    qDebug() << info.addresses();
    //    qDebug() << localName;

    for (QNetworkInterface interface : QNetworkInterface::allInterfaces())
    {
        QList<QNetworkAddressEntry> entryList = interface.addressEntries();

        //遍历每一个IP地址(每个包含一个IP地址，一个子网掩码和一个广播地址)
        foreach(QNetworkAddressEntry entry, entryList)
        {
            if(entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
            {
                //IP地址
                qDebug() << "IP Address:" << entry.ip().toString();

                //子网掩码
                qDebug() << "Netmask:" << entry.netmask().toString();

                //广播地址
                qDebug() << "Broadcast:" << entry.broadcast().toString();
            }
        }
    }
    QByteArray datagram = ui->lineEdit->text().toUtf8();
    sender->writeDatagram(datagram,QHostAddress::Broadcast/*QHostAddress("183.172.255.255")*/, 23333);
    ui->lineEdit->clear();
}

void UdpTest::processPendingDatagrams()
{
    QByteArray datagram;
    while (receiver->hasPendingDatagrams()) {
        datagram.resize(int(receiver->pendingDatagramSize()));
        receiver->readDatagram(datagram.data(), datagram.size());
        ui->textview->setText(ui->textview->text() + "received messages: " + QString(datagram) + "\n");
        ;("received messages: " + QString(datagram));
    }
}
