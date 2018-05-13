#include "ai.h"
#include <QTime>
#include <QCoreApplication>
#include <QDebug>

void Ai::aiPlay()
{
    QTime target = QTime::currentTime().addMSecs(256);
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
    return *(game->possibleMoves.begin());
}

