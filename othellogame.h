#ifndef OTHELLOGAME_H
#define OTHELLOGAME_H

#include <vector>
#include <QPoint>

class OthelloGame
{
public:
    OthelloGame();
    void reset();
    bool isValidMove(int row, int col, int player) const;
    bool makeMove(int row, int col); // انجام حرکت و برگرداندن مهره‌ها
    int getCurrentPlayer() const;
    int getCell(int row, int col) const;
    void switchTurn();
    int getScore(int player) const;
    bool hasValidMove(int player) const; // آیا بازیکن حرکتی دارد؟

private:
    int board[8][8]; // 0: Empty, 1: Black, 2: White
    int currentPlayer;
    // بررسی یک جهت خاص برای محاصره
    bool checkFlip(int row, int col, int dr, int dc, int player, bool applyFlip);
};

#endif // OTHELLOGAME_H
