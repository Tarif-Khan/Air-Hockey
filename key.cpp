#include "key.h"
#include <ncurses.h>

int readInput(bool player1) {
    int ch = getch();
    if (ch == ERR) {
        return -1;
    }

    if (player1) {
        switch (ch) {
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_UP:
            case KEY_DOWN:
            case 'q':
            case 'Q':
                return ch;
            default:
                return -1;
        }
    } else {
        switch (ch) {
            case 'a':
            case 'A':
            case 'd':
            case 'D':
            case 'w':
            case 'W':
            case 's':
            case 'S':
            case 'q':
            case 'Q':
                return ch;
            default:
                return -1;
        }
    }
}