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
    friend class ReversiAi;
    friend class FIRAi;
    void click(int x, int y);
    virtual void undo();
    virtual void init(bool bIsHuman, bool wIsHuman);
    virtual ~Game();

    QPoint vTopLeft;
    QPoint vBottomRight;
    int gridSize;

signals:
    void aiPlay();

protected:
    void drawChess(int x,int y, int player);
    virtual void nextPlayer();
    void saveStatus();
    virtual void showResult() = 0;
    virtual bool canPut(int xpos, int ypos) = 0;
    virtual void put(int xpos, int ypos);
    virtual void calculatePossibleMoves();
    virtual void check();

    //0-Black; 1-White
    int activePlayer = 0;

    bool gameover = false;
    enum PLAYERTYPE{AI,HUMAN};
    QLabel* pictures[9][9];
    int board[9][9];
    int previousMove[100][9][9];
    int previousPlayer[100];
    std::vector<QPoint> possibleMoves;
    int moveCount = -1;
    PLAYERTYPE playerType[2];
    int gridNum;
};

class Reversi:public Game
{
public:
    Reversi(QWidget* parent, QPoint vTL, QLCDNumber* b, QLCDNumber* w);
    void init(bool bIsHuman, bool wIsHuman);
    bool canPut(int xpos, int ypos);
    void put(int xpos, int ypos);
    void showResult();
    void calculateChessNum();
    void undo();
    void hint();
    void hideHint();
    virtual ~Reversi();

private:
    QLCDNumber *black, *white;
};

class FIR:public Game
{
public:
    FIR(QWidget* parent, QPoint vTL);
    bool canPut(int xpos, int ypos);
    void showResult();
    void check(int xpos, int ypos);
    virtual ~FIR();
};

#endif // GAMES_H
