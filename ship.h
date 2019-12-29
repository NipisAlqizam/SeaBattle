#ifndef SHIP_H
#define SHIP_H

struct Ship {
    bool exists;
    int x, y;
    int size;
    bool horizontal;
    Ship(): exists(false), x(0), y(0), size(0), horizontal(false) {}
};

#endif // SHIP_H
