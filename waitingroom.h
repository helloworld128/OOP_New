#ifndef WAITINGROOM_H
#define WAITINGROOM_H

#include <QDialog>
#include <QUdpSocket>

namespace Ui {
class WaitingRoom;
}

class WaitingRoom : public QDialog
{
    Q_OBJECT

public:
    explicit WaitingRoom(QWidget *parent = 0);
    ~WaitingRoom();

signals:
    void PlayReversi();

private slots:
    void processPendingDatagrams();

    void on_Create_Button_clicked();

    void on_Join_Button_clicked();

    void on_Leave_Button_clicked();

    void on_Spectate_Button_clicked();

private:
    QUdpSocket* sender;
    QUdpSocket* receiver;
    Ui::WaitingRoom *ui;
};

#endif // WAITINGROOM_H
