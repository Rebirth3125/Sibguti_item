#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Использование: %s <символ> <скорость> <направление> <цвет>\n", argv[0]);
        return 1;
    }

    char symbol = argv[1][0];
    int speed = atoi(argv[2]);
    int direction = atoi(argv[3]);
    int color = atoi(argv[4]);

    initscr();
    start_color();
    curs_set(0);
    init_pair(1, color, COLOR_BLACK);

    nodelay(stdscr, 1);

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    int x = max_x / 2;
    int y = max_y / 2;

    while (1) {
        clear();
        attron(COLOR_PAIR(1));
        mvaddch(y, x, symbol);
        attroff(COLOR_PAIR(1));
        refresh();

        usleep(speed * 1000);

        x += direction;

        if (x >= max_x || x <= 0) {
            direction = -direction;
        }

        int ch = getch();
        if (ch == 'q') {
            break;
        }
    }

    endwin();

    return 0;
}