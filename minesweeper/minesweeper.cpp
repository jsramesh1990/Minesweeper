#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

const int ROWS = 9;
const int COLS = 9;
const int MINES = 10;

class Minesweeper {
private:
    vector<vector<char>> board;
    vector<vector<bool>> revealed;
    vector<vector<bool>> mineBoard;

public:
    Minesweeper() {
        board.resize(ROWS, vector<char>(COLS, '0'));
        revealed.resize(ROWS, vector<bool>(COLS, false));
        mineBoard.resize(ROWS, vector<bool>(COLS, false));

        placeMines();
        calculateNumbers();
    }

    void placeMines() {
        int placed = 0;

        while (placed < MINES) {
            int r = rand() % ROWS;
            int c = rand() % COLS;

            if (!mineBoard[r][c]) {
                mineBoard[r][c] = true;
                placed++;
            }
        }
    }

    void calculateNumbers() {
        int dr[] = {-1,-1,-1,0,0,1,1,1};
        int dc[] = {-1,0,1,-1,1,-1,0,1};

        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {

                if (mineBoard[r][c]) {
                    board[r][c] = '*';
                    continue;
                }

                int count = 0;

                for (int i = 0; i < 8; i++) {
                    int nr = r + dr[i];
                    int nc = c + dc[i];

                    if (nr >= 0 && nr < ROWS &&
                        nc >= 0 && nc < COLS &&
                        mineBoard[nr][nc]) {
                        count++;
                    }
                }

                board[r][c] = count + '0';
            }
        }
    }

    void printBoard(bool revealAll = false) {
        cout << "\n   ";

        for (int c = 0; c < COLS; c++) {
            cout << c << " ";
        }

        cout << "\n";

        for (int r = 0; r < ROWS; r++) {
            cout << r << "  ";

            for (int c = 0; c < COLS; c++) {

                if (revealAll || revealed[r][c]) {

                    if (board[r][c] == '0')
                        cout << ". ";
                    else
                        cout << board[r][c] << " ";
                }
                else {
                    cout << "# ";
                }
            }

            cout << "\n";
        }
    }

    bool isValid(int r, int c) {
        return r >= 0 && r < ROWS &&
               c >= 0 && c < COLS;
    }

    // Recursive Flood Fill
    void floodFill(int r, int c) {

        if (!isValid(r, c))
            return;

        if (revealed[r][c])
            return;

        if (board[r][c] == '*')
            return;

        revealed[r][c] = true;

        // Stop recursion if number cell
        if (board[r][c] != '0')
            return;

        int dr[] = {-1,-1,-1,0,0,1,1,1};
        int dc[] = {-1,0,1,-1,1,-1,0,1};

        for (int i = 0; i < 8; i++) {
            floodFill(r + dr[i], c + dc[i]);
        }
    }

    bool playMove(int r, int c) {

        if (!isValid(r, c)) {
            cout << "Invalid coordinates!\n";
            return true;
        }

        if (mineBoard[r][c]) {
            cout << "\nBOOM! You hit a mine!\n";
            printBoard(true);
            return false;
        }

        floodFill(r, c);
        return true;
    }

    bool checkWin() {
        int hidden = 0;

        for (int r = 0; r < ROWS; r++) {
            for (int c = 0; c < COLS; c++) {

                if (!revealed[r][c])
                    hidden++;
            }
        }

        return hidden == MINES;
    }
};

int main() {

    srand(time(0));

    Minesweeper game;

    while (true) {

        game.printBoard();

        int row, col;

        cout << "\nEnter row and column: ";
        cin >> row >> col;

        bool alive = game.playMove(row, col);

        if (!alive)
            break;

        if (game.checkWin()) {
            cout << "\nYOU WIN!\n";
            game.printBoard(true);
            break;
        }
    }

    return 0;
}
