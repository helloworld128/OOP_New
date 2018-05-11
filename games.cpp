#include "games.h"
#include "dialog.h"
#include <QDebug>
#include <QDialog>

Game::Game()
{
    gridSize = 60;
}

Game::~Game(){}

void Game::drawChess(int x,int y,int player)
{
    QImage* img;
    if(player == 0) img = new QImage("./images/stoneB.png");
    else img = new QImage("./images/stoneW.png");
    pictures[x][y]->setPixmap(QPixmap::fromImage(*img));
    pictures[x][y]->show();
    board[x][y] = player;
}

void Game::nextPlayer()
{
    if(gameover) return;
    static bool cantmove[2] = {false, false};
    activePlayer = 1 - activePlayer;
    calculatePossibleMoves();
    if(possibleMoves.empty())
    {
        qDebug() << "this player cannot move: Player " + QString::number(activePlayer);
        if(cantmove[1 - activePlayer])
        {
            showResult();
            return;
        }
        cantmove[activePlayer] = true;
        nextPlayer();
    }
    else
    {
        cantmove[activePlayer] = false;
        if(playerType[activePlayer] == AI) emit aiPlay();
    }
}

void Game::click(int x, int y)
{
    if(gameover) return;

    if(canPut(x,y))
    {
        put(x,y);
        saveStatus();
        check();
        nextPlayer();
    }
}

void Reversi::check()
{
    if(black->intValue() + white->intValue() == 64)
        showResult();
}

void Game::saveStatus()
{
    moveCount++;
    for(int i = 0;i < 9;i++)
        for(int j = 0;j < 9;j++)
        {
            previousMove[moveCount][i][j] = board[i][j];
        }
}

void Reversi::undo()
{
    if(moveCount <= 0) return;
    moveCount-=2;
    gameover = false;
    for(int i = 0;i < 9;i++)
    {
        for(int j = 0;j < 9;j++)
        {
            if(previousMove[moveCount][i][j] >= 0)
                drawChess(i,j,previousMove[moveCount][i][j]);
            else
            {
                pictures[i][j]->hide();
                board[i][j] = -1;
            }
        }
    }
    calculateChessNum();
}

void Reversi::showResult()
{
    Dialog* dialog = new Dialog;
    QString s(QString::number(black->intValue()) + " : " + QString::number(white->intValue()) + ", " +
              ((black->intValue() > white->intValue()) ? "black" : "white") + " wins!");
    dialog->setText(s);
    gameover = true;
    dialog->exec();
}

Reversi::Reversi(QWidget* parent, QPoint vTL, QLCDNumber* b, QLCDNumber* w)
    :black(b), white(w)
{
    vTopLeft = vTL;
    vBottomRight = vTL + QPoint(480,480);
    for(int i = 0;i < 9;i++)
        for(int j = 0;j < 9;j++)
        {
            pictures[i][j] = new QLabel(parent);
            pictures[i][j]->move(vTopLeft.x() + i * gridSize, vTopLeft.y() + j * gridSize);
        }
}

Reversi::~Reversi()
{
    black->display(0); white->display(0);
    for(int i = 0;i < 9;i++)
        for(int j = 0;j < 9;j++)
            delete pictures[i][j];
}

void Reversi::init()
{
    for(int i = 0;i < 9;i++)
        for(int j = 0;j < 9;j++)
        {
            board[i][j] = -1;
            pictures[i][j]->hide();
        }
    gameover = false;
    moveCount = 0;
    previousMove[0][3][3] = previousMove[0][4][4] = 0;
    previousMove[0][3][4] = previousMove[0][4][3] = 1;
    activePlayer = 0;
    drawChess(3,3,0);
    drawChess(4,4,0);
    drawChess(3,4,1);
    drawChess(4,3,1);
    black->display(2);
    white->display(2);
}

bool Reversi::canPut(int xpos, int ypos)
{
    if(board[xpos][ypos] == -1)
    {
        int dir[8][2]={{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
        for(int i = 0;i < 8;i++)
        {
            bool flag = false;
            int x = xpos, y = ypos;
            while(0 <= x && x <= 7 && 0 <= y && y <= 7)
            {
                x += dir[i][0]; y += dir[i][1];
                if(!(0 <= x && x <= 7 && 0 <= y && y <= 7)) break;
                if(board[x][y] == -1) break;
                if(board[x][y] == 1 - activePlayer) flag = true;
                if(board[x][y] == activePlayer)
                {
                    if(flag) return true;
                    else break;
                }
            }
        }
    }
    return false;
}

void Reversi::calculateChessNum()
{
    //Calculate chess numbers & display
    int b = 0, w = 0;
    for(int i = 0;i < 8;i++)
        for(int j = 0;j < 8;j++)
        {
            if(board[i][j] == 0) b++;
            if(board[i][j] == 1) w++;
        }
    black->display(b);
    white->display(w);
}

void Reversi::put(int xpos, int ypos)
{
    drawChess(xpos,ypos,activePlayer);
    int reverseList[10][2] = {0};
    int dir[8][2]={{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    for(int i = 0;i < 8;i++)
    {
        int x = xpos, y = ypos;
        int p = 0;
        bool valid = false;
        while(0 <= x && x <= 7 && 0 <= y && y <= 7)
        {
            x += dir[i][0]; y += dir[i][1];
            if(!(0 <= x && x <= 7 && 0 <= y && y <= 7)) break;
            if(board[x][y] != 1 - activePlayer)
            {
                if(board[x][y] == activePlayer) valid = true;
                break;
            }
            reverseList[p][0] = x; reverseList[p][1] = y; p++;
        }
        if(valid)
            for(int j = 0;j < p;j++)
                drawChess(reverseList[j][0],reverseList[j][1],activePlayer);
    }
    calculateChessNum();
}

void Reversi::calculatePossibleMoves()
{
    possibleMoves.clear();
    for(int i = 0;i < 8;i++)
        for(int j = 0;j < 8;j++)
            if(canPut(i,j)) possibleMoves.push_back(QPoint(i,j));
}

FIR::FIR(QWidget* parent)
{

}
