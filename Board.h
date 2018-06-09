#ifndef __Board_h__
#define __Board_h__

class Board
{
public:
    Board();
    Board(const Board&);
    Board& operator=(const Board&);
    ~Board();
    
private:
    char * filled;
    int boardsize;
    int size;
    bool lastplayer;
    int i1;
    
public:
    void play();
    void playvscpu();
    bool setX(const int x, const int y);
    bool setO(const int x, const int y);
    void printBoard();
    void printWinner();
    char whoWon();
    
private:
    int minmax(bool flag);
    int calcH();
    void fillBoard();
    bool boardFull();
    bool isValid(const int x, const int y);
    bool hasWinner();
    bool isWinningRow(const int row);
    bool isWinningCol(const int com);
    bool isMainDiagWin();
    bool isOtDiagWin();
    void clearBoard();
    char getXY(const int x, const int y);
};

#endif /* Board_h */
