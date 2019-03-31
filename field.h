#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <string>

enum State {ST_PLACING_SHIPS, ST_ATTACKING, ST_WAITING, ST_ENDGAME};
class Field
{
public:
    Field(int fieldSize, int seed);
    int fieldSize;
    int maxShipSize;
    std::vector<std::vector<int>> field; /* -1 - 1 клетка до корабля
                                            0  - пусто
                                            [1:maxShipSize] - целый корабль соответствующего размера
                                            [maxShipSize+1:2*maxShip] - 1 попадание
                                            (2*maxSize+2:3*maxSize+2] - убитый корабль */
    State state;
    State *es;
    void autoPlace();
    bool putShip(int y, int x, int size, bool horizontal);
    bool checkShip(int y, int x, int size, bool horizontal);
    bool attack(int x, int y);
    void autoAttack();
    bool isShipHorizontal(int x, int y);
    bool checkKilledShip(int x, int y);
    void killShip(int x, int y);
    void makeField(std::string a);
    bool checkEndGame();

private:
    int seed;
    void printField();

};

#endif // FIELD_H
