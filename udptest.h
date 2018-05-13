#ifndef UDPTEST_H
#define UDPTEST_H

#include <QDialog>
#include <QUdpSocket>

namespace Ui {
class UdpTest;
}

class UdpTest : public QDialog
{
    Q_OBJECT

public:
    explicit UdpTest(QWidget *parent = 0);
    ~UdpTest();

private slots:
    void on_Send_Button_clicked();
    void processPendingDatagrams();

private:
    QUdpSocket* sender;
    QUdpSocket* receiver;
    Ui::UdpTest *ui;
};

#endif // UDPTEST_H
