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
    void drawChess(int x,int y, int player);
    void putChess(const QPoint& pos);
    void saveStatus();
    void undo();
    virtual void init() = 0;
    virtual bool canPut(int xpos, int ypos) = 0;
    virtual void update(int xpos, int ypos) = 0;
    virtual void aiplay() = 0;
    virtual void calculatePossibleMoves() = 0;
    virtual void check() = 0;
    virtual ~Game();


    //0-Black; 1-White
    int activePlayer = 0;

    QPoint vTopLeft;
    QPoint vBottomRight;
    int gridSize;

protected:
    enum MODE{HUMANvsAI,TWOPEOPLE};
    QLabel* pictures[9][9];
    int board[9][9];
    int previousMove[64][9][9];
    std::vector<QPoint> possibleMoves;
    int moveCount = 0;
    MODE mode = HUMANvsAI;
};

class Reversi:public Game
{
public:
    Reversi(QWidget* parent);
    void init();
    bool canPut(int xpos, int ypos);
    void update(int xpos, int ypos);
    void aiplay();
    void calculatePossibleMoves();
    void check();
    virtual ~Reversi();

    QLCDNumber *black, *white;

private:
    bool lastPlayerNotMove = false;
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
