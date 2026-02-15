#include "connectfourgame.h"
#include <cstring> // For memset

ConnectFourGame::ConnectFourGame() {
    reset();
}

void ConnectFourGame::reset() {
    // پر کردن آرایه با 0
    std::memset(board, 0, sizeof(board));
    currentPlayer = 1;
}

int ConnectFourGame::dropPiece(int col) {
    // قانون جاذبه: از پایین‌ترین ردیف شروع کن
    for (int row = 5; row >= 0; --row) {
        if (board[row][col] == 0) {
            board[row][col] = currentPlayer;
            return row; // ردیفی که مهره در آن قرار گرفت
        }
    }
    return -1; // ستون پر است
}

bool ConnectFourGame::checkWin(int row, int col) {
    int player = board[row][col];
    // بررسی 4 جهت: افقی، عمودی، مورب اصلی، مورب فرعی
    return checkDirection(row, col, 0, 1) || // افقی
           checkDirection(row, col, 1, 0) || // عمودی
           checkDirection(row, col, 1, 1) || // مورب
           checkDirection(row, col, 1, -1);  // مورب
}

bool ConnectFourGame::checkDirection(int row, int col, int dRow, int dCol) {
    int player = board[row][col];
    int count = 1;

    // حرکت به جلو
    for (int i = 1; i < 4; ++i) {
        int r = row + i * dRow;
        int c = col + i * dCol;
        if (r < 0 || r >= 6 || c < 0 || c >= 7 || board[r][c] != player) break;
        count++;
    }

    // حرکت به عقب
    for (int i = 1; i < 4; ++i) {
        int r = row - i * dRow;
        int c = col - i * dCol;
        if (r < 0 || r >= 6 || c < 0 || c >= 7 || board[r][c] != player) break;
        count++;
    }

    return count >= 4;
}

int ConnectFourGame::getCurrentPlayer() const { return currentPlayer; }
int ConnectFourGame::getCell(int row, int col) const { return board[row][col]; }
void ConnectFourGame::switchTurn() { currentPlayer = (currentPlayer == 1) ? 2 : 1; }

bool ConnectFourGame::isFull() const {
    for(int j=0; j<7; j++) if(board[0][j] == 0) return false;
    return true;
}
