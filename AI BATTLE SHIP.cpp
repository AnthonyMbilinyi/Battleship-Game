#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int GRID_SIZE = 10;

struct Coordinate {
    int row;
    int col;
};

// Ship structure
struct Ship {
    string name;
    int size;
    vector<Coordinate> coordinates;
    bool isSunk = false;
};

// Grid representation
enum Cell {
    EMPTY,
    SHIP,
    HIT,
    MISS
};

class AI {
private:
    vector<Ship> fleet;
    vector<vector<Cell>> grid;
    vector<Coordinate> targetQueue; // Stores coordinates to focus on after a hit
    vector<vector<bool>> guessed; // Keeps track of already guessed cells

public:
    AI() {
        grid = vector<vector<Cell>>(GRID_SIZE, vector<Cell>(GRID_SIZE, EMPTY));
        guessed = vector<vector<bool>>(GRID_SIZE, vector<bool>(GRID_SIZE, false));
        srand(time(0));
    }

    void placeShips() {
        vector<Ship> ships = {
            {"Carrier", 5},
            {"Battleship", 4},
            {"Cruiser", 3},
            {"Submarine", 3},
            {"Destroyer", 2}
        };

        for (auto& ship : ships) {
            bool placed = false;
            while (!placed) {
                int row = rand() % GRID_SIZE;
                int col = rand() % GRID_SIZE;
                bool horizontal = rand() % 2;

                if (canPlaceShip(row, col, ship.size, horizontal)) {
                    placeShip(row, col, ship.size, horizontal);
                    ship.coordinates = getShipCoordinates(row, col, ship.size, horizontal);
                    fleet.push_back(ship);
                    placed = true;
                }
            }
        }
    }

    Coordinate makeMove() {
        if (!targetQueue.empty()) {
            Coordinate nextTarget = targetQueue.back();
            targetQueue.pop_back();
            return nextTarget;
        }

        // Random guessing for unexplored cells
        int row, col;
        do {
            row = rand() % GRID_SIZE;
            col = rand() % GRID_SIZE;
        } while (guessed[row][col]);

        guessed[row][col] = true;
        return {row, col};
    }

    void processResult(Coordinate coord, bool hit) {
        if (hit) {
            grid[coord.row][coord.col] = HIT;

            // Add adjacent cells to the target queue
            for (auto& delta : vector<Coordinate>{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}) {
                int newRow = coord.row + delta.row;
                int newCol = coord.col + delta.col;

                if (isValidCoordinate(newRow, newCol) && !guessed[newRow][newCol]) {
                    targetQueue.push_back({newRow, newCol});
                }
            }
        } else {
            grid[coord.row][coord.col] = MISS;
        }
    }

private:
    bool canPlaceShip(int row, int col, int size, bool horizontal) {
        for (int i = 0; i < size; ++i) {
            int r = row + (horizontal ? 0 : i);
            int c = col + (horizontal ? i : 0);

            if (!isValidCoordinate(r, c) || grid[r][c] != EMPTY) {
                return false;
            }
        }
        return true;
    }

    void placeShip(int row, int col, int size, bool horizontal) {
        for (int i = 0; i < size; ++i) {
            int r = row + (horizontal ? 0 : i);
            int c = col + (horizontal ? i : 0);
            grid[r][c] = SHIP;
        }
    }

    vector<Coordinate> getShipCoordinates(int row, int col, int size, bool horizontal) {
        vector<Coordinate> coordinates;
        for (int i = 0; i < size; ++i) {
            coordinates.push_back({row + (horizontal ? 0 : i), col + (horizontal ? i : 0)});
        }
        return coordinates;
    }

    bool isValidCoordinate(int row, int col) {
        return row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE;
    }
};

int main() {
    AI aiPlayer;
    aiPlayer.placeShips();

    cout << "AI ships have been placed!" << endl;

    while (true) {
        Coordinate move = aiPlayer.makeMove();
        cout << "AI attacks: (" << move.row << ", " << move.col << ")" << endl;

        // Simulate result (replace this with actual game logic)
        bool hit = (rand() % 2 == 0);
        aiPlayer.processResult(move, hit);

        if (hit) {
            cout << "AI hit a ship!" << endl;
        } else {
            cout << "AI missed!" << endl;
        }
    }

    return 0;
}

