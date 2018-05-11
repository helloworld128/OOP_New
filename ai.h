#ifndef AI_H
#define AI_H

#include "games.h"

class Ai:public QObject
{
    Q_OBJECT
public:
    Ai(Game* g):game(g){}
protected:
    Game* game;

protected slots:
    virtual void aiPlay() = 0;
};

class ReversiAi:public Ai
{
public:
    ReversiAi(Game* g):Ai(g){}

protected:
    void aiPlay();
};

#endif // AI_H
