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
    void reStart();
    virtual void undo();
    virtual void init(bool bIsHuman, bool wIsHuman);
    virtual void nextPlayer();
    virtual void showResult() = 0;
    virtual ~Game();

    QPoint vTopLeft;
    QPoint vBottomRight;
    int gridSize;
    bool waiting = false;
    int moveCount = -1;
    int activePlayer = 0;      //0-Black; 1-White
    int previousMove[100][9][9];
    int isOnlineGame = false;

signals:
    void aiPlay();
    void sendPut(int x, int y);

public slots:
    void click(int x, int y);

protected slots:
    void opponentReady();

protected:
    void drawChess(int x,int y, int player);
    void saveStatus();
    virtual void put(int xpos, int ypos);
    virtual bool canPut(int xpos, int ypos) = 0;
    virtual void calculatePossibleMoves();
    virtual void check();


    bool gameover = false;
    enum PLAYERTYPE{AI,HUMAN,ONLINE};
    QLabel* pictures[9][9];
    int board[9][9];
    int previousPlayer[100];
    std::vector<QPoint> possibleMoves;
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
        bool judgeRepeat(int xpos, int ypos, int** Board);
        bool stillalive(int xpos, int ypos, int Player, int** _board);
        void eliminate(int xpos, int ypos, int Player);
        virtual ~Go();
};

#endif // GAMES_H
