#ifndef PUCK_H
#define PUCK_H

#include "slider.h"

class Puck {
private:
    int x;
    int y;
    int dx;
    int dy;

public:
    Puck(int x, int y);
    void move(int fieldStartX, int fieldStartY, int fieldWidth, int fieldHeight);
    void checkSliderCollision(const Slider& slider);
    void checkWallCollision(int fieldStartX, int fieldStartY, int fieldWidth, int fieldHeight);
    void draw();
    void clear();
    int getX() const;
    int getY() const;
    void reset(int x, int y);
};

#endif