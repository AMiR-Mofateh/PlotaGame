#ifndef CONNECTFOURGAME_H
#define CONNECTFOURGAME_H

#include <vector>

class ConnectFourGame
{
public:
    ConnectFourGame();
    void reset();
    // تلاش برای انداختن مهره در ستون col. برمی‌گرداند کدام ردیف پر شد (-1 یعنی ستون پر است)
    int dropPiece(int col);
    bool checkWin(int row, int col);
    int getCurrentPlayer() const; // 1 or 2
    int getCell(int row, int col) const;
    bool isFull() const;
    void switchTurn();

private:
    int board[6][7]; // 0: Empty, 1: Player1, 2: Player2
    int currentPlayer;
    bool checkDirection(int row, int col, int dRow, int dCol);
};

#endif // CONNECTFOURGAME_H
