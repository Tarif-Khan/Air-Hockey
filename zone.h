#ifndef ZONE_H
#define ZONE_H

class Zone {
private:
    int width;
    int height;
    int startX;
    int startY;
    int goalWidth;

public:
    Zone(int width, int height, int startX, int startY, int goalWidth);
    void draw();
};

#endif