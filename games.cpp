#include "games.h"
#include "dialog.h"
#include <QDebug>
#include <QDialog>

Game::Game(QLabel* _currentPlayerPict)
{
    currentPlayerPict = _currentPlayerPict;
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

    //it's bad practice, but necessary to implement hints for reversi
    auto ptr = dynamic_cast<Reversi*>(this);
    if(ptr && playerType[activePlayer] == HUMAN) ptr->hideHint();

    static bool cantmove[2] = {false, false};
    activePlayer = 1 - activePlayer;
    QImage* img;
    if(activePlayer == 0) img = new QImage("./images/stoneB.png");
    else img = new QImage("./images/stoneW.png");
    currentPlayerPict->setPixmap(QPixmap::fromImage(*img));
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

        if(ptr && playerType[activePlayer] == HUMAN) ptr->hint();
    }
}

void Game::click(int x, int y)
{
    if(gameover) return;

    if(canPut(x,y))
    {
        put(x,y);
        saveStatus();
        if(auto ptr = dynamic_cast<FIR*>(this)) ptr->check(x,y);
        else check();
        nextPlayer();
    }
}

void Game::put(int xpos, int ypos)
{
    drawChess(xpos,ypos,activePlayer);
}

void Game::calculatePossibleMoves()
{
    possibleMoves.clear();
    for(int i = 0;i < gridNum;i++)
        for(int j = 0;j < gridNum;j++)
            if(canPut(i,j)) possibleMoves.push_back(QPoint(i,j));
}

void Game::undo()
{
    int recover = moveCount;
    while(moveCount > 0 && playerType[previousPlayer[moveCount]] != HUMAN)
        moveCount--;
    moveCount--;
    if(moveCount < 0)
    {
        moveCount = recover;
        return;
    }
    activePlayer = moveCount ? (1 - previousPlayer[moveCount]) : 0;
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
}

void Game::saveStatus()
{
    moveCount++;
    previousPlayer[moveCount] = activePlayer;
    for(int i = 0;i < 9;i++)
        for(int j = 0;j < 9;j++)
        {
            previousMove[moveCount][i][j] = board[i][j];
        }
}

void Game::init(bool bIsHuman, bool wIsHuman)
{
    for(int i = 0;i < 9;i++)
        for(int j = 0;j < 9;j++)
        {
            board[i][j] = -1;
            pictures[i][j]->hide();
        }
    playerType[0] = bIsHuman ? HUMAN : AI;
    playerType[1] = wIsHuman ? HUMAN : AI;
    gameover = false;
    moveCount = -1;
    activePlayer = 0;
    QImage* img = new QImage("./images/stoneB.png");
    currentPlayerPict->setPixmap(QPixmap::fromImage(*img));
    saveStatus();
    if(playerType[0] == AI)
    {
        calculatePossibleMoves();
        emit aiPlay();
    }
}

void Game::check()
{
    for(int i = 0;i < gridNum;i++)
        for(int j = 0;j < gridNum;j++)
            if(board[i][j] == -1) return;
    showResult();
}

void Reversi::hint()
{
    QImage* img = new QImage("./images/hint.png");
    for(auto it = possibleMoves.begin();it != possibleMoves.end();it++)
    {
        pictures[it->x()][it->y()]->setPixmap(QPixmap::fromImage(*img));
        pictures[it->x()][it->y()]->show();
    }
}

void Reversi::hideHint()
{
    for(auto it = possibleMoves.begin();it != possibleMoves.end();it++)
    {
        int x = it->x(), y = it->y();
        if(board[x][y] >= 0) drawChess(x,y,board[x][y]);
        else pictures[x][y]->hide();
    }
}

void Reversi::undo()
{
    Game::undo();
    if(playerType[activePlayer] == HUMAN)
    {
        calculatePossibleMoves();
        hint();
    }
    calculateChessNum();
}

void Reversi::showResult()
{
    Dialog* dialog = new Dialog;
    QString s(QString::number(black->intValue()) + " : " + QString::number(white->intValue()) + ", ");
    if(black->intValue() > white->intValue()) s += "Black wins!";
    else if(black->intValue() < white->intValue()) s += "White wins!";
    else s += "Draw!";
    dialog->setText(s);
    gameover = true;
    dialog->exec();
}

Reversi::Reversi(QWidget* parent, QPoint vTL, QLCDNumber* b, QLCDNumber* w, QLabel* _currentPlayerPict)
    :black(b), white(w), Game(_currentPlayerPict)
{
    gridNum = 8;
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

void Reversi::init(bool bIsHuman, bool wIsHuman)
{
    for(int i = 0;i < 9;i++)
        for(int j = 0;j < 9;j++)
        {
            board[i][j] = -1;
            pictures[i][j]->hide();
        }
    playerType[0] = bIsHuman ? HUMAN : AI;
    playerType[1] = wIsHuman ? HUMAN : AI;
    gameover = false;
    moveCount = -1;
    drawChess(3,3,0);
    drawChess(4,4,0);
    drawChess(3,4,1);
    drawChess(4,3,1);
    black->display(2);
    white->display(2);
    activePlayer = 0;
    QImage* img = new QImage("./images/stoneB.png");
    currentPlayerPict->setPixmap(QPixmap::fromImage(*img));
    saveStatus();
    calculatePossibleMoves();
    if(playerType[0] == AI)
    {
        emit aiPlay();
    }
    else hint();
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
    Game::drawChess(xpos,ypos,activePlayer);
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

FIR::FIR(QWidget* parent, QPoint vTL, QLabel *_currentPlayerPict) : Game(_currentPlayerPict)
{
    gridNum = 9;
    vTopLeft = vTL - QPoint(30,30);
    vBottomRight = vTL + QPoint(540,540);
    for(int i = 0;i < 9;i++)
        for(int j = 0;j < 9;j++)
        {
            pictures[i][j] = new QLabel(parent);
            pictures[i][j]->move(vTopLeft.x() + i * gridSize, vTopLeft.y() + j * gridSize);
        }
}

bool FIR::canPut(int xpos, int ypos)
{
    if(board[xpos][ypos] == -1) return true;
    else return false;
}

void FIR::showResult()
{
    Dialog* dialog = new Dialog;
    QString s(QString(activePlayer ? "White" : "Black") + QString(" wins!"));
    dialog->setText(s);
    gameover = true;
    dialog->exec();
}

void FIR::check(int xpos, int ypos)
{
    Game::check();
    bool gameEndFlag = false;
    int dir[8][2]={{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    for(int i = 0;i < 8;i++)
    {
        int x = xpos, y = ypos;
        int count = 0;
        while(0 <= x && x <= 8 && 0 <= y && y <= 8)
        {
            if(board[x][y] == activePlayer) count++;
            else break;
            x += dir[i][0]; y += dir[i][1];
        }
        if(count >= 5)
        {
            gameEndFlag = true;
            break;
        }
    }
    if(gameEndFlag) showResult();
}

FIR::~FIR()
{
    for(int i = 0;i < 9;i++)
        for(int j = 0;j < 9;j++)
            delete pictures[i][j];
}
