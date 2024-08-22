#ifndef SLIDER_H
#define SLIDER_H

class Slider {
private:
    int x;
    int y;
    int size;

public:
    Slider(int x, int y, int size);
    void move(int ch, int fieldStartX, int fieldStartY, int fieldWidth, int fieldHeight);
    void clear();
    void draw();
    int getX() const;
    int getY() const;
    int getSize() const;
};

#endif