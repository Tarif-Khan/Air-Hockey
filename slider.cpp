#include "slider.h"
#include <ncurses.h>

Slider::Slider(int x, int y, int size) : x(x), y(y), size(size) {}

void Slider::move(int ch, int fieldStartX, int fieldStartY, int fieldWidth, int fieldHeight) {
    switch (ch) {
        case KEY_LEFT:
        case 'a':
        case 'A':
            if (x > fieldStartX + 1) x--;
            break;
        case KEY_RIGHT:
        case 'd':
        case 'D':
            if (x < fieldStartX + fieldWidth - size - 1) x++;
            break;
        case KEY_UP:
        case 'w':
        case 'W':
            if (y > fieldStartY + 1) y--;
            break;
        case KEY_DOWN:
        case 's':
        case 'S':
            if (y < fieldStartY + fieldHeight - 2) y++;
            break;
    }
}

void Slider::clear() {
    for (int i = 0; i < size; i++) {
        mvprintw(y, x + i, " ");
    }
}

void Slider::draw() {
    attron(COLOR_PAIR(4));
    for (int i = 0; i < size; i++) {
        mvprintw(y, x + i, "=");
    }
    attroff(COLOR_PAIR(4));
}

int Slider::getX() const {
    return x;
}

int Slider::getY() const {
    return y;
}

int Slider::getSize() const {
    return size;
}