#include "ai.h"
#include <QTime>
#include <QCoreApplication>

void ReversiAi::aiPlay()
{
    QTime target = QTime::currentTime().addMSecs(100);
    while(QTime::currentTime() < target)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
    int x = game->possibleMoves[0].x(), y = game->possibleMoves[0].y();
    game->click(x,y);
}

