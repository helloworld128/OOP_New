#include "ai.h"
#include <QTime>
#include <QCoreApplication>
#include <QDebug>

void Ai::aiPlay()
{
    QTime target = QTime::currentTime().addMSecs(250);
    while(QTime::currentTime() < target)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
    QPoint decision = evaluate();
    game->click(decision.x(),decision.y());
}

QPoint ReversiAi::evaluate()
{
    static int valueTable[8][8] = {
        {90, -60, 10, 10, 10, 10, -60, 90},
        {-60, -80, 5, 5, 5, 5, -80, -60},
        {10, 5, 1, 1, 1, 1, 5, 10},
        {10, 5, 1, 1, 1, 1, 5, 10},
        {10, 5, 1, 1, 1, 1, 5, 10},
        {10, 5, 1, 1, 1, 1, 5, 10},
        {-60, -80, 5, 5, 5, 5, -80, -60},
        {90, -60, 10, 10, 10, 10, -60, 90}};
    if(game->possibleMoves.empty()) qDebug() << "There is no possible move.";
    auto it = game->possibleMoves.begin();
    auto max = it;
    for(; it != game->possibleMoves.end();it++)
    {
        if(valueTable[it->x()][it->y()] > valueTable[max->x()][max->y()])
            max = it;
    }
    return *max;
}

QPoint FIRAi::evaluate()
{
    int scoreTable[9][9] = {0};
    int tupleScoreTable[10] = {7, 35, 800, 15000, 800000, 15, 400, 1800, 100000, 0};

    for (int xpos = 0; xpos < 9; xpos++)
        for (int ypos = 0; ypos < 9; ypos++){//for every point on the board
            if (game->board[xpos][ypos] != -1) scoreTable[xpos][ypos] = -1;
            else{
                for (int i = 0; i < 5; i++){
                    int bcount = 0, wcount = 0;
                    int yBegin = ypos + i;//y's five groups' begining
                    if (yBegin >= 4 && yBegin <= 8){
                        for (int k = 0; k < 5; k++){
                            int temp = game->board[xpos][yBegin - k];
                            if (temp == 0) bcount++;
                            if (temp == 1) wcount++;
                        }
                        if (wcount == 0)
                           scoreTable[xpos][ypos] += tupleScoreTable[bcount];
                        else if (bcount == 0 && wcount != 0)
                            scoreTable[xpos][ypos] += tupleScoreTable[4 + wcount];
                    //    else if (wcount != 0 && bcount != 0) scoreTable[xpos][ypos] = 0;
                    }
                }
                for (int i = 0; i < 5; i++){
                    int bcount = 0, wcount = 0;//x's five groups' beginning
                    int xBegin = xpos + i;
                    if (xBegin >= 4 && xBegin <= 8){
                        for (int k = 0; k < 5; k++){
                            int temp = game->board[xBegin - k][ypos];
                            if (temp == 0) bcount++;
                            if (temp == 1) wcount++;
                        }
                        if (wcount == 0)
                           scoreTable[xpos][ypos] += tupleScoreTable[bcount];
                        else if (bcount == 0 && wcount != 0)
                            scoreTable[xpos][ypos] += tupleScoreTable[4 + wcount];
                    //    else if (wcount != 0 && bcount != 0) scoreTable[xpos][ypos] = 0;
                    }
                }
                for (int i = 0; i < 5; i++){
                    int bcount = 0, wcount = 0;//left to right
                    int xBegin = xpos + i, yBegin = ypos + i;
                    if (xBegin >= 4 && xBegin <= 8 && yBegin >= 4 && yBegin <= 8){
                        for (int k = 0; k < 5; k++){
                            int temp = game->board[xBegin - k][yBegin - k];
                            if (temp == 0) bcount++;
                            if (temp == 1) wcount++;
                        }
                        if (wcount == 0)
                           scoreTable[xpos][ypos] += tupleScoreTable[bcount];
                        else if (bcount == 0 && wcount != 0)
                            scoreTable[xpos][ypos] += tupleScoreTable[4 + wcount];
                    //    else if (wcount != 0 && bcount != 0) scoreTable[xpos][ypos] = 0;
                    }
                }
                for (int i = 0; i < 5; i++){
                    int bcount = 0, wcount = 0;//right to left
                    int xBegin = xpos - i, yBegin = ypos + i;
                    if (xBegin >= 0 && xBegin <= 4 && yBegin >= 4 && yBegin <= 8){
                        for (int k = 0; k < 5; k++){
                            int temp = game->board[xBegin + k][yBegin - k];
                            if (temp == 0) bcount++;
                            if (temp == 1) wcount++;
                        }
                        if (wcount == 0)
                           scoreTable[xpos][ypos] += tupleScoreTable[bcount];
                        else if (bcount == 0 && wcount != 0)
                            scoreTable[xpos][ypos] += tupleScoreTable[4 + wcount];
                     //   else if (wcount != 0 && bcount != 0) scoreTable[xpos][ypos] = 0;
                    }
                }
            }
        }
    /*for (int i = 0; i < 9; i++){
        for (int j = 0; j < 9; j++)
    qDebug() << scoreTable[i][j] << " ";
    qDebug() << '\n';
    }*/

    auto it = game->possibleMoves.begin();
    auto max = it;
    for(; it != game->possibleMoves.end();it++)
    {
        if(scoreTable[it->x()][it->y()] > scoreTable[max->x()][max->y()])
            max = it;
    }
    return *max;
}

