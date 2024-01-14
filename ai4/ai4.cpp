#include <vector>
#include <iostream>
#include <climits>
#include <Windows.h> 
#include <algorithm> 
#include <iomanip>

const int ROWS = 6; // строки
const int COLS = 7; // столбцы 

int evaluateSequence(int sequence, int emptyCells) {
    int score = 0;
    if (sequence == 4)
        score += 10000;
    else if (sequence == 3 && emptyCells == 1)
        score += 1000;
    else if (sequence == 2 && emptyCells == 2)
        score += 100;

    return score;
}


int evaluate(std::vector<std::vector<int>>& matrix) {

    int score = 0;
    int player = 1; // это комп
    int player_opponent = 0; // это я

    // горизонт компа
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 4; ++col) {
            int sequence = 0;
            int emptyCells = 0;
            for (int i = 0; i < 4; ++i) {
                if (matrix[row][col + i] == player)
                    sequence++;
                else if (matrix[row][col + i] == 2)
                    emptyCells++;
            }
            score += evaluateSequence(sequence, emptyCells);
        }
    }

    // горизонт игрока
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 4; ++col) {
            int sequence = 0;
            int emptyCells = 0;
            for (int i = 0; i < 4; ++i) {
                if (matrix[row][col + i] == player_opponent)
                    sequence++;
                else if (matrix[row][col + i] == 2)
                    emptyCells++;
            }
            score -= evaluateSequence(sequence, emptyCells);
        }
    }

    // вертикаль компа
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 7; ++col) {
            int sequence = 0;
            int emptyCells = 0;
            for (int i = 0; i < 4; ++i) {
                if (matrix[row + i][col] == player)
                    sequence++;
                else if (matrix[row + i][col] == 2)
                    emptyCells++;
            }

            score += evaluateSequence(sequence, emptyCells);
        }
    }

    // вертикаль игрока
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 7; ++col) {
            int sequence = 0;
            int emptyCells = 0;
            for (int i = 0; i < 4; ++i) {
                if (matrix[row + i][col] == player_opponent)
                    sequence++;
                else if (matrix[row + i][col] == 2)
                    emptyCells++;
            }

            score -= evaluateSequence(sequence, emptyCells);
        }
    }

    // гл лиагональ компа
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 4; ++col) {
            int sequence = 0;
            int emptyCells = 0;
            for (int i = 0; i < 4; ++i) {
                if (matrix[row + i][col + i] == player)
                    sequence++;
                else if (matrix[row + i][col + i] == 2)
                    emptyCells++;
            }

            score += evaluateSequence(sequence, emptyCells);
        }
    }

    // гл лиагональ игрока
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 4; ++col) {
            int sequence = 0;
            int emptyCells = 0;
            for (int i = 0; i < 4; ++i) {
                if (matrix[row + i][col + i] == player_opponent)
                    sequence++;
                else if (matrix[row + i][col + i] == 2)
                    emptyCells++;
            }

            score -= evaluateSequence(sequence, emptyCells);
        }
    }

    // диагональ компа
    for (int row = 3; row < 6; ++row) {
        for (int col = 0; col < 4; ++col) {
            int sequence = 0;
            int emptyCells = 0;
            for (int i = 0; i < 4; ++i) {
                if (matrix[row - i][col + i] == player)
                    sequence++;
                else if (matrix[row - i][col + i] == 2)
                    emptyCells++;
            }
            score += evaluateSequence(sequence, emptyCells);
        }
    }

    // диагональ игрока
    for (int row = 3; row < 6; ++row) {
        for (int col = 0; col < 4; ++col) {
            int sequence = 0;
            int emptyCells = 0;
            for (int i = 0; i < 4; ++i) {
                if (matrix[row - i][col + i] == player_opponent)
                    sequence++;
                else if (matrix[row - i][col + i] == 2)
                    emptyCells++;
            }
            score -= evaluateSequence(sequence, emptyCells);
        }
    }
    //std::cout << score << '\n';
    return score;
}


bool isValidMove(std::vector<std::vector<int>>& matrix, int col) {
    return matrix[0][col] == 2;
}


void makeMove(std::vector<std::vector<int>>& matrix, int col, int player) {
    for (int row = ROWS - 1; row >= 0; --row) {
        if (matrix[row][col] == 2) {
            matrix[row][col] = player;
            break;
        }
    }
}

void undoMove(std::vector<std::vector<int>>& matrix, int col) {
    for (int row = 0; row < ROWS; ++row) {
        if (matrix[row][col] != 2) {
            matrix[row][col] = 2;
            break;
        }
    }
}

int alphabeta(std::vector<std::vector<int>>& matrix, int depth, int alpha, int beta, bool maximizingPlayer) {
    if (depth == 0 || evaluate(matrix) >= 10000 || evaluate(matrix) <= -10000) {
        return evaluate(matrix);
    }

    if (maximizingPlayer) {
        int maxEval = INT_MIN;
        for (int col = 0; col < COLS; ++col) {
            if (isValidMove(matrix, col)) {
                makeMove(matrix, col, 1);
                int eval = alphabeta(matrix, depth - 1, alpha, beta, false);
                maxEval = max(maxEval, eval);
                alpha = max(alpha, maxEval);
                undoMove(matrix, col);
                if (beta <= alpha) {
                    break;
                }
            }
        }
        return maxEval;
    }
    else {
        int minEval = INT_MAX;
        for (int col = 0; col < COLS; ++col) {
            if (isValidMove(matrix, col)) {
                makeMove(matrix, col, 0);
                int eval = alphabeta(matrix, depth - 1, alpha, beta, true);
                minEval = min(minEval, eval);
                beta = min(beta, minEval);
                undoMove(matrix, col);
                if (beta <= alpha) {
                    break;
                }
            }
        }
        return minEval;
    }
}


int findBestMove(std::vector<std::vector<int>>& matrix, int depth) {
    int bestMove = -1;
    int maxEval = INT_MIN;
    for (int col = 0; col < COLS; ++col) {
        if (isValidMove(matrix, col)) {
            makeMove(matrix, col, 1); // 1
            int eval = alphabeta(matrix, depth - 1, INT_MIN, INT_MAX, false);
            undoMove(matrix, col);
            if (eval > maxEval) {
                maxEval = eval;
                bestMove = col;
            }
        }
    }
    return bestMove;
}

void printMatrix(std::vector<std::vector<int>> matrix) {
    for (size_t i = 0; i < matrix.size(); i++) {
        for (size_t j = 0; j < matrix[i].size(); j++) {
            std::cout << std::setw(3) << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void players_move(std::vector<std::vector<int>>& matrix, int move, int player) {
    if (matrix[0][move] == 2) {
        for (int i = 1; i < matrix.size(); i++)
        {
            if (matrix[i][move] != 2) {
                matrix[i - 1][move] = player;
                break;
            }
        }
        if (matrix[matrix.size() - 1][move] == 2)
            matrix[matrix.size() - 1][move] = player;
    }
    else
    {
        std::cout << "Ход не выполнен" << '\n';
    }
}

int win_playes(std::vector<std::vector<int>> matrix) {
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 4; ++col) {
            int player = 0;
            int player_opponent = 0;
            for (int i = 0; i < 4; ++i) {
                if (matrix[row][col + i] == 1)
                    player++;
                else if (matrix[row][col + i] == 0)
                    player_opponent++;
            }
            if (player == 4)
                return 1;
            if (player_opponent == 4)
                return -1;
        }
    }

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 7; ++col) {
            int player = 0;
            int player_opponent = 0;
            for (int i = 0; i < 4; ++i) {
                if (matrix[row + i][col] == 1)
                    player++;
                else if (matrix[row + i][col] == 0)
                    player_opponent++;
            }
            if (player == 4)
                return 1;
            if (player_opponent == 4)
                return -1;
        }
    }

    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 4; ++col) {
            int player = 0;
            int player_opponent = 0;
            for (int i = 0; i < 4; ++i) {
                if (matrix[row + i][col + i] == 1)
                    player++;
                else if (matrix[row + i][col + i] == 0)
                    player_opponent++;
            }
            if (player == 4)
                return 1;
            if (player_opponent == 4)
                return -1;
        }
    }

    for (int row = 3; row < 6; ++row) {
        for (int col = 0; col < 4; ++col) {
            int player = 0;
            int player_opponent = 0;
            for (int i = 0; i < 4; ++i) {
                if (matrix[row - i][col + i] == 1)
                    player++;
                else if (matrix[row - i][col + i] == 0)
                    player_opponent++;
            }
            if (player == 4)
                return 1;
            if (player_opponent == 4)
                return -1;
        }
    }

    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 7; ++col) {
            int player = 0;
            for (int i = 0; i < 4; ++i) {
                if (matrix[row - i][col + i] != 2)
                    player++;
            }
            if (player == 42)
                return 2;
        }
    }
    return 0;
}
int main() {
    SetConsoleOutputCP(1251);
    std::vector<std::vector<int>> matrix = {
            {2, 2, 2, 2, 2, 2, 2},
            {2, 2, 2, 2, 2, 2, 2},
            {2, 2, 2, 2, 2, 2, 2},
            {2, 2, 2, 2, 2, 2, 2},
            {2, 2, 2, 2, 2, 2, 2},
            {2, 2, 2, 2, 2, 2, 2}
    };

    int move;
    int temp;
    int win;
    while (true)
    {



        int bestMove = findBestMove(matrix, 9);
        std::cout << "Компьютер: " << std::endl;
        players_move(matrix, bestMove, 1);
        printMatrix(matrix);
        win = win_playes(matrix);
        std::cout << bestMove;
        if (win == -1) {
            std::cout << "Победил 1";
            break;
        }
        if (win == 1)
        {
            std::cout << "Победил 0";
            break;
        }
        if (win == 2)
        {
            std::cout << "Ничья";
            break;
        }

        //int bestMove = findBestMove(matrix, 7);
        std::cout << "Введите номер строки: ";
        std::cin >> move;
        /*if (move == -1) {
            undoMove(matrix, temp);
            printMatrix(matrix);
            continue;
        }*/
        //temp = bestMove;
        std::cout << "Игрок: " << std::endl;
        players_move(matrix, move, 0);
        printMatrix(matrix);
        win = win_playes(matrix);
        if (win == -1) {
            std::cout << "Победил 1";
            break;
        }
        if (win == 1)
        {
            std::cout << "Победил 0";
            break;
        }
        if (win == 2)
        {
            std::cout << "Ничья";
            break;
        }
    }

    return 0;
}
