#include "puck.h"
#include <ncurses.h>
#include <cstdlib>

Puck::Puck(int x, int y) : x(x), y(y), dx(1), dy(1) {}

void Puck::move(int fieldStartX, int fieldStartY, int fieldWidth, int fieldHeight) {
    if (rand() % 3 == 0) {  
        x += dx;
        y += dy;
    }

   
    if (x <= fieldStartX + 1) {
        x = fieldStartX + 2;
        dx = -dx;
    } else if (x >= fieldStartX + fieldWidth - 2) {
        x = fieldStartX + fieldWidth - 3;
        dx = -dx;
    }

    
    int goalWidth = fieldWidth / 4;
    int goalStartX = fieldStartX + (fieldWidth - goalWidth) / 2;
    if (y <= fieldStartY + 1 && (x < goalStartX || x >= goalStartX + goalWidth)) {
        y = fieldStartY + 2;
        dy = -dy;
    } else if (y >= fieldStartY + fieldHeight - 2 && (x < goalStartX || x >= goalStartX + goalWidth)) {
        y = fieldStartY + fieldHeight - 3;
        dy = -dy;
    }
}

void Puck::checkSliderCollision(const Slider& slider) {
    if (x >= slider.getX() && x < slider.getX() + slider.getSize() &&
        y == slider.getY()) {
        dy = -dy;
    }
}

void Puck::checkWallCollision(int fieldStartX, int fieldStartY, int fieldWidth, int fieldHeight) {
    if (x <= fieldStartX + 1 || x >= fieldStartX + fieldWidth - 2) {
        dx = -dx;
    }
    if (y <= fieldStartY + 1 || y >= fieldStartY + fieldHeight - 2) {
        dy = -dy;
    }
}

void Puck::draw() {
    attron(COLOR_PAIR(1));
    mvprintw(y, x, "O");
    attroff(COLOR_PAIR(1));
}

void Puck::clear() {
    mvprintw(y, x, " ");
}

int Puck::getX() const {
    return x;
}

int Puck::getY() const {
    return y;
}

void Puck::reset(int newX, int newY) {
    x = newX;
    y = newY;
    dx = (rand() % 2 == 0) ? 1 : -1;
    dy = (rand() % 2 == 0) ? 1 : -1;
}
