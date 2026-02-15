#include "othellogame.h"
#include <cstring>

OthelloGame::OthelloGame() {
    reset();
}

void OthelloGame::reset() {
    std::memset(board, 0, sizeof(board));
    // چینش اولیه استاندارد
    board[3][3] = 2; // سفید
    board[4][4] = 2; // سفید
    board[3][4] = 1; // سیاه
    board[4][3] = 1; // سیاه
    currentPlayer = 1; // همیشه سیاه شروع می‌کند
}

bool OthelloGame::isValidMove(int row, int col, int player) const {
    if (board[row][col] != 0) return false;

    // بررسی 8 جهت
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            // متد checkFlip را فقط برای بررسی صدا می‌زنیم (false)
            if (const_cast<OthelloGame*>(this)->checkFlip(row, col, dr, dc, player, false)) return true;
        }
    }
    return false;
}

bool OthelloGame::makeMove(int row, int col) {
    if (!isValidMove(row, col, currentPlayer)) return false;

    board[row][col] = currentPlayer;
    // اعمال تغییر رنگ در تمام جهات
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) continue;
            checkFlip(row, col, dr, dc, currentPlayer, true);
        }
    }
    return true;
}

bool OthelloGame::checkFlip(int row, int col, int dr, int dc, int player, bool applyFlip) {
    int r = row + dr;
    int c = col + dc;
    int opponent = (player == 1) ? 2 : 1;
    bool seenOpponent = false;

    // حرکت در جهت خاص تا رسیدن به دیوار یا مهره خودی
    while (r >= 0 && r < 8 && c >= 0 && c < 8) {
        if (board[r][c] == opponent) {
            seenOpponent = true;
        } else if (board[r][c] == player) {
            if (seenOpponent) {
                // محاصره انجام شد
                if (applyFlip) {
                    // برگشتن به عقب و تغییر رنگ‌ها
                    int backR = r - dr;
                    int backC = c - dc;
                    while (backR != row || backC != col) {
                        board[backR][backC] = player;
                        backR -= dr;
                        backC -= dc;
                    }
                }
                return true;
            }
            return false;
        } else {
            return false; // خانه خالی
        }
        r += dr;
        c += dc;
    }
    return false;
}

int OthelloGame::getCurrentPlayer() const { return currentPlayer; }
int OthelloGame::getCell(int row, int col) const { return board[row][col]; }
void OthelloGame::switchTurn() { currentPlayer = (currentPlayer == 1) ? 2 : 1; }

int OthelloGame::getScore(int player) const {
    int score = 0;
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (board[i][j] == player) score++;
    return score;
}

bool OthelloGame::hasValidMove(int player) const {
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j)
            if (isValidMove(i, j, player)) return true;
    return false;
}
