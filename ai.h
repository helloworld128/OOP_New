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
    virtual void aiPlay();
    virtual QPoint evaluate() = 0;
};

class ReversiAi:public Ai
{
public:
    ReversiAi(Game* g):Ai(g){}

protected:
    QPoint evaluate();
};

class FIRAi:public Ai
{
public:
    FIRAi(Game* g):Ai(g){}

protected:
    QPoint evaluate();
};

#endif // AI_H
