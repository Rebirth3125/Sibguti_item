#include <ncurses.h>
#include <string>

int main() {
    initscr();
    int height, width;
    getmaxyx(stdscr, height, width);

    std::string message = "HELLO WORLD";

    int x = (width - message.length()) / 2;
    int y = height / 2;

    mvprintw(y, x, "%s", message.c_str());

    refresh();

    getch();

    endwin();

    return 0;
}