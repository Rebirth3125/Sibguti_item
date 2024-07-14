#include <ncurses.h>

int main() 
{   
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    curs_set(1);

    printw("Press keys (q - exit):\n");

    int a;
    
    while ((a = getch()) != 'q') {
        clear();
        printw("Code keys: %d\n", a);
        refresh();
    }

    endwin();

    return 0;
}