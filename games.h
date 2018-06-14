#ifndef GAMES_H
#define GAMES_H
#include <QPoint>
#include <QLabel>
#include <QLCDNumber>
#include <QMediaPlayer>
#include <QMovie>
#include <QVector>

class Game:public QObject
{
    Q_OBJECT
public:
    Game(QLabel* _currentPlayerPict);
    friend class ReversiAi;
    friend class FIRAi;
    void reStart(int count, int active, int*** record, QPoint* moves);
    void setPlayerType(int arg);
    void click(int x, int y);
    virtual void undo();
    virtual void init(bool bIsHuman, bool wIsHuman);
    virtual void nextPlayer();
    void showResult();
    virtual ~Game();

    QPoint vTopLeft;
    QPoint vBottomRight;
    int gridSize;
    bool waiting = false;
    int moveCount = -1;
    int activePlayer = 0;      //0-Black; 1-White
    int previousMove[100][9][9];
    QPoint previousMovePoint[100];
    int isOnlineGame = false;
    int hasWatcher = false;
    bool ready[2] = {false, false};
    enum PLAYERTYPE{AI,HUMAN,ONLINE};
    PLAYERTYPE playerType[2];

signals:
    void aiPlay();
    void sendPut(int x, int y);
    void sendWatchPut(int x, int y);
    void sendNotice(const QString& text);
    void resetReady();

protected slots:
    void startGame();
    void opponentPut(int x, int y);
    void watchPut(int x, int y);
    void opponentLeft();

protected:
    void drawChess(int x,int y, int player);
    void saveStatus();
    void playSound();
    virtual void put(int xpos, int ypos);
    virtual bool canPut(int xpos, int ypos) = 0;
    virtual void calculatePossibleMoves();
    virtual void check();
    virtual QString generateResultStr() = 0;

    bool gameover = false;
    QLabel* pictures[9][9];
    QLabel* lastMoveHint[9][9];
    int board[9][9];
    std::vector<QPoint> possibleMoves;
    int previousPlayer[100];
    int gridNum;
    QLabel* currentPlayerPict;
    QMediaPlayer* soundPlayer;
};

class Reversi:public Game
{
public:
    Reversi(QWidget* parent, QPoint vTL, QLCDNumber* b, QLCDNumber* w, QLabel *_currentPlayerPict);
    void init(bool bIsHuman, bool wIsHuman);
    bool canPut(int xpos, int ypos);
    void put(int xpos, int ypos);
    void calculateChessNum();
    void undo();
    void hint();
    void hideHint();
    virtual ~Reversi();

private:
    QString generateResultStr();
    QLCDNumber *black, *white;
    QVector<QMovie*> movies;
};

class FIR:public Game
{
public:
    FIR(QWidget* parent, QPoint vTL, QLabel* _currentPlayerPict);
    bool canPut(int xpos, int ypos);
    void check(int xpos, int ypos);
    virtual ~FIR();
private:
    QString generateResultStr();
    bool FullFlag;
};

class Go:public Game
{
public:
    Go(QWidget* parent, QPoint vTL, QLabel* _currentPlayerPict);
    bool canPut(int xpos, int ypos);
    void put(int xpos, int ypos);
    virtual ~Go();
private:
    QString generateResultStr();
    bool judgeRepeat(int xpos, int ypos, int** Board);
    bool stillalive(int xpos, int ypos, int Player, int** _board);
    void eliminate(int xpos, int ypos, int Player);
};

#endif // GAMES_H
