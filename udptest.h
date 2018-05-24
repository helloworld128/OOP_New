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

signals:
    void PlayReversi();

private slots:
    void processPendingDatagrams();

    void on_Create_Button_clicked();

    void on_Join_Button_clicked();

    void on_Leave_Button_clicked();

private:
    QUdpSocket* sender;
    QUdpSocket* receiver;
    Ui::UdpTest *ui;
};

#endif // UDPTEST_H
