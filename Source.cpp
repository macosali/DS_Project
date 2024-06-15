#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>

using namespace std;

const int SIZE = 14;
const int UNREACHABLE_COUNT = 20;
const char UNREACHABLE = 'U';
const char EMPTY = 'e';
const char START = 'S';
const char TARGET = 'T';
const char WIRE = 'W';

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
};

class PCB {
public:
    PCB();
    void displayGrid();
    bool findShortestPath();

private:
    vector<vector<char>> grid;
    Point start;
    Point target;
    bool isValid(int row, int col);
    void placeRandomTiles();
};

PCB::PCB() : grid(SIZE, vector<char>(SIZE, EMPTY)) {
    srand(static_cast<unsigned int>(time(0)));
    placeRandomTiles();
}

void PCB::placeRandomTiles() {
    // Place unreachable tiles
    for (int i = 0; i < UNREACHABLE_COUNT; ++i) {
        int x, y;
        do {
            x = rand() % SIZE;
            y = rand() % SIZE;
        } while (grid[x][y] != EMPTY);
        grid[x][y] = UNREACHABLE;
    }

    // Place starting tile
    do {
        start.x = rand() % SIZE;
        start.y = rand() % SIZE;
    } while (grid[start.x][start.y] != EMPTY);
    grid[start.x][start.y] = START;

    // Place target tile
    do {
        target.x = rand() % SIZE;
        target.y = rand() % SIZE;
    } while (grid[target.x][target.y] != EMPTY);
    grid[target.x][target.y] = TARGET;
}

bool PCB::isValid(int row, int col) {
    return (row >= 0) && (row < SIZE) && (col >= 0) && (col < SIZE) && (grid[row][col] == EMPTY || grid[row][col] == TARGET);
}

void PCB::displayGrid() {
    for (const auto& row : grid) {
        for (const auto& tile : row) {
            cout << tile << "\t";
        }
        cout << endl;
    }
}

bool PCB::findShortestPath() {
    vector<Point> directions = { Point(0, 1), Point(1, 0), Point(0, -1), Point(-1, 0) };
    queue<pair<Point, vector<Point>>> q;
    q.push({ start, {start} });

    // Create a 2D vector to keep track of visited points
    vector<vector<bool>> visited(SIZE, vector<bool>(SIZE, false));
    visited[start.x][start.y] = true;

    while (!q.empty()) {
        auto current = q.front();
        Point pt = current.first;
        vector<Point> path = current.second;
        q.pop();

        if (pt.x == target.x && pt.y == target.y) {
            // Mark the path on the grid
            for (const auto& p : path) {
                if (grid[p.x][p.y] != START && grid[p.x][p.y] != TARGET) {
                    grid[p.x][p.y] = WIRE;
                }
            }
            return true;
        }

        for (const auto& direction : directions) {
            int newX = pt.x + direction.x;
            int newY = pt.y + direction.y;

            if (isValid(newX, newY) && !visited[newX][newY]) {
                visited[newX][newY] = true;
                vector<Point> newPath = path;
                newPath.push_back(Point(newX, newY));
                q.push({ Point(newX, newY), newPath });
            }
        }
    }
    return false;
}

int main() {
    PCB pcb;
    cout << "Initial grid:" << endl;
    pcb.displayGrid();
    cout << endl;

    if (pcb.findShortestPath()) {
        cout << "Shortest path found:" << endl;
    }
    else {
        cout << "No path found." << endl;
    }

    pcb.displayGrid();

    return 0;
}
