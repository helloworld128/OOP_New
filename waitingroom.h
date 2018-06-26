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
    void createGame(int type, int side, QString localName, QString otherName = "");
    void opponentPut(int x, int y);
    void watchPut(int x, int y);
    void opponentEntered(QString name);
    void opponentLeft();
    void opponentGiveUp();
    void opponentStopOnce();
    void startGame();
    void opponentChat(QString text);
    void spectate(int type, int side, QString nameb, QString namew);
    void requestBoard();
    void sendBoard(int **board, int currentPlayer);

private slots:
    void connected();
    void sendGameFinished();
    void readData();
    void sendMove(int x, int y);
    void sendReady();
    void sendQuit();
    void sendGiveUp();
    void sendStopOnce();
    void sendText(QString text);
    void on_Create_Button_clicked();
    void on_Join_Button_clicked();
    void on_Leave_Button_clicked();
    void on_Spectate_Button_clicked();
    void on_Cancel_Button_clicked();
    void on_OK_Button_clicked();
    void on_lineEdit_editingFinished();
    void on_Refresh_Button_clicked();
    void receiveBoard(int** board, int currentPlayer);

    void on_IP_editingFinished();

private:
    void requestCurrentGames();
    void addGame(int type, QString nameb, QString namew, int uid);
    Ui::WaitingRoom *ui;
    QTcpSocket* socket = nullptr;
    QButtonGroup games, role;
    QString playerName;
    bool b_connected = false;
    bool isWatching = false;
};

#endif // WAITINGROOM_H
