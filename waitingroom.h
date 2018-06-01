#ifndef WAITINGROOM_H
#define WAITINGROOM_H

#include <QDialog>
#include <QTcpSocket>
#include <QButtonGroup>

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
    void createGame(int type, int side, QString name);

private slots:
    void readData();

    void on_Create_Button_clicked();

    void on_Join_Button_clicked();

    void on_Leave_Button_clicked();

    void on_Spectate_Button_clicked();

    void on_Cancel_Button_clicked();

    void on_OK_Button_clicked();

    void on_lineEdit_editingFinished();

    void on_Refresh_Button_clicked();

private:
    void requestCurrentGames();
    Ui::WaitingRoom *ui;
    QTcpSocket* socket = nullptr;
    QButtonGroup games, role;
};

#endif // WAITINGROOM_H
