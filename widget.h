#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QLCDNumber>
#include <QButtonGroup>
#include "games.h"
#include "onlinemodule.h"
#include "util.h"
#include <QMessageBox>
#include <QTranslator>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

    Ui::Widget *ui;
    Game* game = nullptr;
    Notice* notice = nullptr;
    OnlineModule* hall = nullptr;
    void mousePressEvent(QMouseEvent *);
    QButtonGroup selectBPlayer,selectWPlayer;

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

signals:
    void sendReady();
    void sendQuit();
    void sendGiveUp();
    void sendStopOnce();
    void sendText(QString text);

private slots:
    void createGame(int type, int side, QString localName, QString otherName);
    void setGameUI(int isOnline, int gameType);
    void setOpponentName(QString name);
    void opponentChat(QString text);
    void displayNotice(const QString& text);
    void resetReady();
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
    void on_GiveUp_Button_2_clicked();
    void on_StopOnce_Button_clicked();
    void on_StopOnce_Button_2_clicked();
    void on_Quit_Button_clicked();
    void on_Ready_Button_clicked();
    void on_Chatting_Button_clicked();
    void on_SendText_Button_clicked();
    void on_Close_Chatting_Button_clicked();
    void on_Language_Button_clicked();
};

#endif // WIDGET_H
