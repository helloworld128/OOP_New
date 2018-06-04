#include "games.h"
#include "dialog.h"
#include "util.h"
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
    setPicture(pictures[x][y], (player == 0) ? BLACKCHESS : WHITECHESS);
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
    if (playerType[activePlayer] != HUMAN) waiting = true;
    else waiting = false;
    setPicture(currentPlayerPict, (activePlayer == 0) ? BLACKCHESS : WHITECHESS);
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
    if (isOnlineGame) emit sendPut(xpos, ypos);
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
    setPicture(currentPlayerPict, BLACKCHESS);
    saveStatus();
    if(playerType[0] != HUMAN) waiting = true;
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


void Game::reStart()
{
    for (int x = 0; x < 9; x++)
        for (int y = 0; y < 9; y++)
        {
            if (previousMove[moveCount][x][y] == 0)
                setPicture(pictures[x][y], BLACKCHESS);
            if (previousMove[moveCount][x][y] == 1)
                setPicture(pictures[x][y], WHITECHESS);
            if (previousMove[moveCount][x][y] == -1)
                pictures[x][y]->hide();
        }

    nextPlayer();
}

void Reversi::hint()
{
    for(auto it = possibleMoves.begin();it != possibleMoves.end();it++) {
        setPicture(pictures[it->x()][it->y()], HINT);
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
    setPicture(currentPlayerPict, BLACKCHESS);
    saveStatus();
    calculatePossibleMoves();
    if(playerType[0] != HUMAN) waiting = true;
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
    FullFlag = true;
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
    if (!FullFlag){
        QString s(QString(activePlayer ? "White" : "Black") + QString(" wins!"));
        dialog->setText(s);
    }

    else {
        QString s(QString("Draw!"));
        dialog->setText(s);
    }
    gameover = true;
    dialog->exec();
}

void FIR::check(int xpos, int ypos)
{
    bool gameEndFlag = false;
    int dir[4][2]={{1, 0},{1, -1},{0, -1},{1, 1}};
    for(int i = 0; i < 4; i++)
    {
        int x = xpos, y = ypos;
        int count = 0;
        while(0 <= x && x <= 8 && 0 <= y && y <= 8 && board[x][y] == activePlayer){
            count++;
            x += dir[i][0]; y += dir[i][1];
        }
        x = xpos - dir[i][0]; y = ypos - dir[i][1];
        while(0 <= x && x <= 8 && 0 <= y && y <= 8 && board[x][y] == activePlayer){
            count++;
            x -= dir[i][0]; y -= dir[i][1];
        }
        if(count >= 5)
        {
            gameEndFlag = true;
            FullFlag = false;
            break;
        }
    }
    if (!gameEndFlag){
        bool flag = false;
        for (int i = 0; i < 9; i++){
            for (int j = 0; j < 9; j++){
                if (board[i][j] == -1){
                    FullFlag = false;
                    flag = true;
                    break;
                }
            }
            if (flag) break;
        }
        if (!flag) {
            FullFlag = true;
            gameEndFlag = true;
        }
    }
    if (gameEndFlag) showResult();
}

FIR::~FIR()
{
    for(int i = 0;i < 9;i++)
        for(int j = 0;j < 9;j++)
            delete pictures[i][j];
}


Go::Go(QWidget* parent, QPoint vTL, QLabel* _currentPlayerPict): Game(_currentPlayerPict)
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

Go::~Go()
{
    for(int i = 0;i < 9;i++)
        for(int j = 0;j < 9;j++)
            delete pictures[i][j];
}

bool Go::judgeRepeat(int xpos, int ypos, int** _board)
{
    int dir[4][2]={{1,0},{0,-1},{-1,0},{0,1}};
    for (int d = 0; d < 4; d++)
    {
        int x = xpos + dir[d][0], y = ypos + dir[d][1];
        if (x < 0 || x >= 9 || y < 0 || y >= 9||_board[x][y] != 1 - activePlayer)
            continue;
        bool canEliminate = true;
        int q[90][2] = {0};
        bool visited[9][9] = {0};
        int f = -1, r = 0;
        q[0][0] = x, q[0][1] = y, visited[x][y] = true;
        while (f < r && canEliminate)
        {
            f++;
            for (int i = 0 ; i < 4; i++)
            {
                int xx = q[f][0] + dir[i][0], yy = q[f][1] + dir[i][1];
                if (0 <= xx && xx < 9 && 0 <= yy && yy < 9)
                {
                    if (_board[xx][yy] == -1) canEliminate = false;
                    if (_board[xx][yy] == 1 - activePlayer && !visited[xx][yy])
                    {
                        r++;
                        q[r][0] = xx;
                        q[r][1] = yy;
                        visited[xx][yy] = true;
                    }
                }
            }
        }
        if (canEliminate)
            for (int t = 0; t <= r; t++)
            {
                int x = q[t][0], y = q[t][1];
                _board[x][y] = -1;
            }
    }


    for (int t = 0; t <= moveCount; t++)
    {
        bool flag = false;
        if (previousPlayer[t] == activePlayer) flag = true;
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                if (previousMove[t][i][j] != _board[i][j]) flag = false;
        if (flag) return true;
    }

    return false;
}

bool Go::stillalive(int xpos, int ypos, int Player, int** _board)
{
    int dir[4][2]={{1,0},{0,-1},{-1,0},{0,1}};
    int q[90][2];
    bool visited[9][9] = {0};
    int f = -1, r = 0;
    q[0][0] = xpos, q[0][1] = ypos, visited[xpos][ypos] = true;
    while (f < r)
    {
        f++;
        for (int i = 0 ; i < 4; i++)
        {
            int x = q[f][0] + dir[i][0], y = q[f][1] + dir[i][1];
            if (0 <= x && x < 9 && 0 <= y && y < 9)
            {
                if (_board[x][y] == -1) return true;
                if (_board[x][y] == Player && !visited[x][y])
                {
                    r++;
                    q[r][0] = x;
                    q[r][1] = y;
                    visited[x][y] = true;
                }
            }
        }
    }
    return false;
}

bool Go::canPut(int xpos, int ypos)
{
    if(board[xpos][ypos] != -1) return false;

    bool flag = false;

    int dir[4][2]={{1,0},{0,-1},{-1,0},{0,1}};
    int **_board = new int*[9];
    for (int i = 0; i < 9 ; i++ ) _board[i] = new int[9];
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) _board[i][j] = board[i][j];
    _board[xpos][ypos] = activePlayer;

    if (stillalive(xpos, ypos, activePlayer, _board)) flag = true;
    else for (int i = 0; i < 4; i++)
    {
        int x = xpos + dir[i][0], y = ypos + dir[i][1];
        if (0 <= x && x < 9 && 0 <= y && y < 9 &&
                !stillalive(x,y,1-activePlayer,_board))
            flag = true;
    }

   if (judgeRepeat(xpos, ypos, _board)) flag = false;

    for (int i = 0; i < 9 ; i++ ) delete[] _board[i];
    delete[] _board;

    return flag;
}

void Go::eliminate(int xpos, int ypos, int Player)
{
    int dir[4][2]={{1,0},{0,-1},{-1,0},{0,1}};
    int q[90][2];
    bool visited[9][9] = {0};
    int f = -1, r = 0;
    q[0][0] = xpos, q[0][1] = ypos, visited[xpos][ypos] = true;
    while (f < r)
    {
        f++;
        for (int i = 0 ; i < 4; i++)
        {
            int x = q[f][0] + dir[i][0], y = q[f][1] + dir[i][1];
            if (0 <= x && x < 9 && 0 <= y && y < 9)
            {
                if (board[x][y] == -1) return;
                if (board[x][y] == Player && !visited[x][y])
                {
                    r++;
                    q[r][0] = x;
                    q[r][1] = y;
                    visited[x][y] = true;
                }
            }
        }
    }
    for (int t = 0; t <= r; t++)
    {
        int xx = q[t][0], yy = q[t][1];
         board[xx][yy] = -1;
         pictures[xx][yy]->hide();
    }
}

void Go::put(int xpos, int ypos)
{
    Game::drawChess(xpos,ypos,activePlayer);
    int dir[4][2]={{1,0},{0,-1},{-1,0},{0,1}};
    for (int i = 0; i < 4; i++)
    {
        int x = xpos + dir[i][0], y = ypos + dir[i][1];
        if (0 <= x && x < 9 && 0 <= y && y < 9 && board[x][y] == 1-activePlayer) eliminate(x,y,1-activePlayer);
    }
}

void Go::showResult()
{
    Dialog* dialog = new Dialog;
    QString s("Game Finished!");
    dialog->setText(s);
    gameover = true;
    dialog->exec();
}
