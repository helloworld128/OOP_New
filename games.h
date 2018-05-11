#ifndef GAMES_H
#define GAMES_H
#include <QPoint>
#include <QLabel>
#include <QLCDNumber>

class Game:public QObject
{
    Q_OBJECT
public:
    Game();
    friend class Ai;
    friend class ReversiAi;
    void drawChess(int x,int y, int player);
    void click(int x, int y);
    virtual void nextPlayer();
    void saveStatus();
    virtual void undo() = 0;
    virtual void showResult() = 0;
    virtual void init() = 0;
    virtual bool canPut(int xpos, int ypos) = 0;
    virtual void put(int xpos, int ypos) = 0;
    virtual void calculatePossibleMoves() = 0;
    virtual void check() = 0;
    virtual ~Game();

    QPoint vTopLeft;
    QPoint vBottomRight;
    int gridSize;

signals:
    void aiPlay();

protected:
    //0-Black; 1-White
    int activePlayer = 0;

    bool gameover = false;
    enum PLAYERTYPE{AI,HUMAN};
    QLabel* pictures[9][9];
    int board[9][9];
    int previousMove[100][9][9];
    std::vector<QPoint> possibleMoves;
    int moveCount = 0;
    PLAYERTYPE playerType[2] = {HUMAN,AI};
};

class Reversi:public Game
{
public:
    Reversi(QWidget* parent, QPoint vTL, QLCDNumber* b, QLCDNumber* w);
    void init();
    bool canPut(int xpos, int ypos);
    void put(int xpos, int ypos);
    //void nextPlayer();
    void aiplay();
    void calculatePossibleMoves();
    void check();
    void showResult();
    void calculateChessNum();
    void undo();
    virtual ~Reversi();

    QLCDNumber *black, *white;
};

class FIR:public Game
{
public:
    FIR(QWidget* parent);
//    void init();
//    bool canPut(int xpos, int ypos);
//    void update(int xpos, int ypos);
//    void aiplay();
//    void check();
//    virtual ~FIR();
};

#endif // GAMES_H
