#ifndef GAMES_H
#define GAMES_H
#include <QPoint>
#include <QLabel>
#include <QLCDNumber>

class Game:public QObject
{
    Q_OBJECT
public:
    Game(QLabel* _currentPlayerPict);
    friend class ReversiAi;
    friend class FIRAi;
    void click(int x, int y);
    virtual void undo();
    virtual void init(bool bIsHuman, bool wIsHuman);
    virtual void nextPlayer();
    virtual void showResult() = 0;
    virtual ~Game();

    QPoint vTopLeft;
    QPoint vBottomRight;
    int gridSize;
    bool waiting = false;

signals:
    void aiPlay();

protected:
    void drawChess(int x,int y, int player);
    void saveStatus();
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
    QLabel* currentPlayerPict;
};

class Reversi:public Game
{
public:
    Reversi(QWidget* parent, QPoint vTL, QLCDNumber* b, QLCDNumber* w, QLabel *_currentPlayerPict);
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
    FIR(QWidget* parent, QPoint vTL, QLabel* _currentPlayerPict);
    bool canPut(int xpos, int ypos);
    void showResult();
    void check(int xpos, int ypos);
    virtual ~FIR();
private:
    bool FullFlag;
};

class Go:public Game
{
public:
    Go(QWidget* parent, QPoint vTL, QLabel* _currentPlayerPict);
        bool canPut(int xpos, int ypos);
        void put(int xpos, int ypos);
        void showResult();
        bool judgeRepeat(const int Board[9][9]);
        bool stillalive(int xpos, int ypos, int Player,const int _board[9][9]);
        void eliminate(int xpos, int ypos, int Player);
        virtual ~Go();
};

#endif // GAMES_H
