#include "zone.h"
#include <ncurses.h>

Zone::Zone(int width, int height, int startX, int startY, int goalWidth)
    : width(width), height(height), startX(startX), startY(startY), goalWidth(goalWidth) {}

void Zone::draw() {
    attron(COLOR_PAIR(5));  
    for (int i = 1; i < width - 1; i++) {
        mvprintw(startY, startX + i, "-");
        mvprintw(startY + height - 1, startX + i, "-");
    }

    
    for (int i = 1; i < height - 1; i++) {
        mvprintw(startY + i, startX, "|");
        mvprintw(startY + i, startX + width - 1, "|");
    }
    attroff(COLOR_PAIR(5));

    
    attron(COLOR_PAIR(2));
    for (int i = 1; i < width - 1; i++) {
        mvprintw(startY + height / 2, startX + i, "-");
    }
    attroff(COLOR_PAIR(2));

    
    int goalStartX = (width - goalWidth) / 2;
    int goalEndX = goalStartX + goalWidth;
    for (int i = goalStartX; i < goalEndX; i++) {
        mvprintw(startY, startX + i, " ");
        mvprintw(startY + height - 1, startX + i, " ");
    }
}
