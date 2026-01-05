#include <ncurses.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

enum Dir { UP, DOWN, LEFT, RIGHT };

class Board {
public:
    int w, h;

    Board(int width, int height) {
        w = width;
        h = height;
    }

    void draw() {
        for (int x = 0; x < w; x++) {
            mvaddch(0, x, '#');
            mvaddch(h - 1, x, '#');
        }

        for (int y = 0; y < h; y++) {
            mvaddch(y, 0, '#');
            mvaddch(y, w - 1, '#');
        }
    }
};

class Food {
public:
    int x, y;

    Food() {
        x = 1;
        y = 1;
    }

    void respawn(Board &b, vector<pair<int,int>> &snake) {
        while (true) {
            x = rand() % (b.w - 2) + 1;
            y = rand() % (b.h - 2) + 1;

            bool bad = false;
            for (int i = 0; i < snake.size(); i++) {
                if (snake[i].first == x && snake[i].second == y) {
                    bad = true;
                    break;
                }
            }
            if (!bad) break;
        }
    }

    void draw() {
        mvaddch(y, x, 'O');
    }
};

class Snake {
public:
    vector<pair<int,int>> body;
    Dir dir;
    bool growNext;

    Snake(int x, int y) {
        dir = RIGHT;
        growNext = false;

        body.push_back({x, y});
        body.push_back({x - 1, y});
        body.push_back({x - 2, y});
    }

    void changeDir(Dir d) {
        if ((dir == UP && d == DOWN) ||
            (dir == DOWN && d == UP) ||
            (dir == LEFT && d == RIGHT) ||
            (dir == RIGHT && d == LEFT))
            return;

        dir = d;
    }

    void move() {
        pair<int,int> head = body[0];

        if (dir == UP) head.second--;
        if (dir == DOWN) head.second++;
        if (dir == LEFT) head.first--;
        if (dir == RIGHT) head.first++;

        body.insert(body.begin(), head);

        if (!growNext)
            body.pop_back();
        else
            growNext = false;
    }

    void grow() {
        growNext = true;
    }

    bool hitSelf() {
        for (int i = 1; i < body.size(); i++) {
            if (body[i] == body[0])
                return true;
        }
        return false;
    }

    void draw() {
        for (int i = 0; i < body.size(); i++) {
            if (i == 0)
                mvaddch(body[i].second, body[i].first, '@');
            else
                mvaddch(body[i].second, body[i].first, 'o');
        }
    }
};

class Game {
public:
    Board board;
    Snake snake;
    Food food;
    bool running;
    int score;

    Game() : board(40, 20), snake(20, 10) {
        running = true;
        score = 0;
        food.respawn(board, snake.body);
    }

    void input() {
        int ch = getch();

        if (ch == 'w') snake.changeDir(UP);
        if (ch == 's') snake.changeDir(DOWN);
        if (ch == 'a') snake.changeDir(LEFT);
        if (ch == 'd') snake.changeDir(RIGHT);
        if (ch == 'q') running = false;
    }

    void update() {
        snake.move();

        int x = snake.body[0].first;
        int y = snake.body[0].second;

        if (x <= 0 || x >= board.w - 1 || y <= 0 || y >= board.h - 1)
            running = false;

        if (snake.hitSelf())
            running = false;

        if (x == food.x && y == food.y) {
            snake.grow();
            score += 10;
            food.respawn(board, snake.body);
        }
    }

    void draw() {
        clear();
        board.draw();
        snake.draw();
        food.draw();
        mvprintw(board.h, 0, "Score: %d", score);
        refresh();
    }
};

int main() {
    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr, TRUE);

    srand(time(0));

    while (true) {
        Game game;

        while (game.running) {
            game.input();
            game.update();
            game.draw();
            napms(100);
        }

        clear();
        mvprintw(10, 10, "GAME OVER");
        mvprintw(12, 10, "Score: %d", game.score);
        mvprintw(14, 10, "Press R to restart or Q to quit");
        refresh();

        nodelay(stdscr, FALSE);
        char c = getch();
        if (c == 'q') break;
        nodelay(stdscr, TRUE);
    }

    endwin();
    return 0;
}
