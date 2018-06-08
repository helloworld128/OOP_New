#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QLCDNumber>
#include <QButtonGroup>
#include "games.h"
#include "waitingroom.h"
#include "util.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

    Ui::Widget *ui;

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    Game* game = nullptr;
    Notice* notice = nullptr;
    WaitingRoom* hall = nullptr;

protected:
    void mousePressEvent(QMouseEvent *);

private:
    QButtonGroup selectBPlayer,selectWPlayer;
    void changeOnlineUI();

signals:
    void sendReady();

private slots:

    void createGame(int type, int side, QString localName, QString otherName);
    void setGameUI(int isOnline, int gameType);
    void setOpponentName(QString name);
    void on_Start_Button_clicked();
    void on_Reversi_Button_clicked();
    void on_FIR_Button_clicked();
    void on_Go_Button_clicked();
    void on_Undo_Button_clicked();
    void on_Menu_Button_clicked();
    void on_Save_Button_clicked();
    void on_Load_Button_clicked();
    void on_Online_Button_clicked();
    void on_Local_Button_clicked();
    void on_Back_Button_clicked();
    void on_GiveUp_Button_clicked();
    void on_StopOnce_Button_clicked();
    void on_Quit_Button_clicked();
    void on_Ready_Button_clicked();
    void on_pushButton_clicked();
};

#endif // WIDGET_H
