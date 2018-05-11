#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QLCDNumber>
#include "games.h"

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

    Game* game;

protected:
    void mousePressEvent(QMouseEvent *);

private:
    void hideHomeScreenUI();
    void showHomeScreenUI();

private slots:

    void on_Start_Button_clicked();
    void on_Reversi_Button_clicked();
    void on_FIR_Button_clicked();
    void on_Go_Button_clicked();
    void on_Undo_Button_clicked();
    void on_Menu_Button_clicked();
};

#endif // WIDGET_H
