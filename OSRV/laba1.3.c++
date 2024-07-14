#include <ncurses.h>
#include <unistd.h> 

int main() {
    initscr();          
    cbreak();           
    noecho();           
    curs_set(0);       
    nodelay(stdscr, 1); 

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    int x = max_x / 2;  
    int y = max_y / 2;
    int dx = 1;         
    int dy = 1;         
    int symbol = 'X';   

    while (1) {
        clear();            
        mvaddch(y, x, symbol); 
        refresh();         

        usleep(100000);    

        x += dx;
        y += dy;

        if (x >= max_x || x <= 0) {
            dx = -dx; 
        }
        if (y >= max_y || y <= 0) {
            dy = -dy; 
        }

        int ch = getch(); 
        if (ch == 'q') {
            break; 
        }
    }

    endwin(); 

    return 0;
}