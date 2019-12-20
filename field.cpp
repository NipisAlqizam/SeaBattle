#include "field.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>
//#define DEBUG
//#define DTM
Field::Field(int fieldSize, int seed)
    :fieldSize(fieldSize),
      maxShipSize(4),
      field(10, std::vector<int>(fieldSize, 0)),
      state(ST_PLACING_SHIPS),
      seed(seed)
{
    srand(seed);
//    autoPlace();
//    putShip(4, 9, 4, false);
//    putShip(9, 9, 1, false);
//    makeField("  1 -1 -1 -1 -1  0 -1  2 -1  0\
//              -1 -1  2  2 -1  0 -1  2 -1  0\
//               0 -1 -1 -1 -1 -1 -1 -1 -1 -1\
//               0  0 -1 -1 -1 -1  3  3  3 -1\
//               0  0 -1  2 -1 -1 -1 -1 -1 -1\
//               0  0 -1  2 -1 -1  1 -1 -1  4\
//              -1 -1 -1 -1 -1 -1 -1 -1 -1  4\
//              -1  3 -1  0  0  0  0  0 -1  4\
//              -1  3 -1 -1 -1 -1  0  0 10 10\
//              -1  3 -1 -1  1 -1  0  0 10 11");
//    putShip(5, 9, 4, false);
//    putShip(9, 9, 1, true);
//    state = ST_ATTACKING;
    state = ST_PLACING_SHIPS;
    printField();
}

void Field::makeField(std::string a)
{
    std::stringstream ss(a);
    field = std::vector<std::vector<int>>(10, std::vector<int>(fieldSize, 0));
    for (int i = 0; i<fieldSize; ++i)
    {
        for (int j = 0; j<fieldSize; ++j)
        {
            ss >> field[i][j];
        }
    }
}


bool Field::checkShip(int y, int x, int size, bool horizontal)
{
    if (x < 0 || y < 0)
        return false;
    if (!horizontal)
    {
        if (y+size>fieldSize)
            return false;
        for (int i = y; i<y+size; ++i)
        {
            if (field[i][x] != 0)
                return false;
        }
    } else
    {
        if (x+size>fieldSize)
            return false;
        for (int i = x; i<x+size; ++i)
        {
            if (field[y][i] != 0)
                return false;
        }
    }
    return true;
}

bool Field::putShip(int y, int x, int size, bool horizontal)
{
    if (!checkShip(y, x, size, horizontal))
        return false;
    if (horizontal)
    {
        if (x-1 >= 0)
        {
            field[y][x-1] = -1;
            if (y-1 >= 0)
                field[y-1][x-1] = -1;
            if (y+1 < fieldSize)
                field[y+1][x-1] = -1;
        }
        if (x+size < fieldSize)
        {
            field[y][x+size] = -1;
            if (y-1 >= 0)
                field[y-1][x+size] = -1;
            if (y+1 < fieldSize)
                field[y+1][x+size] = -1;
        }
        for (int i = x; i<x+size; ++i)
        {
            field[y][i] = size;
            if (y+1<fieldSize)
                field[y+1][i] = -1;
            if (y-1 >= 0)
                field[y-1][i] = -1;
        }
    } else
    {
        if (y-1 >= 0)
        {
            field[y-1][x] = -1;
            if (x-1 >= 0)
                field[y-1][x-1] = -1;
            if (x+1 < fieldSize)
                field[y-1][x+1] = -1;
        }
        if (y+size < fieldSize)
        {
            field[y+size][x] = -1;
            if (x-1 >= 0)
                field[y+size][x-1] = -1;
            if (x+1 < fieldSize)
                field[y+size][x+1] = -1;
        }
        for (int i = y; i<y+size; ++i)
        {
            field[i][x] = size;
            if (x+1<fieldSize)
                field[i][x+1] = -1;
            if (x-1 >= 0)
                field[i][x-1] = -1;
        }
    }
    return true;
}

void Field::printField()
{
#ifdef DEBUG
    for (int k = 0; k<10; ++k)
    {
        for (int l = 0; l<10; ++l)
        {
            std::cout << std::right << std::setw(3) << field[k][l];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
#endif
}

void Field::autoPlace()
{
    int x, y;
    bool hor;
    bool ch, cv;
    for (int i = 0; i<maxShipSize; ++i)
    {
        for (int j = 0; j<i+1; ++j)
        {
            ch = 0;
            cv = 0;
            do
            {
                do
                {
                    x = rand()%fieldSize;
                    y = rand()%fieldSize;
                } while(field[y][x] != 0);
                ch = checkShip(y, x, maxShipSize-i, true);
                cv = checkShip(y, x, maxShipSize-i, false);
                if (ch && cv)
                    hor = rand()%2;
                else if (ch ^ cv)
                    hor = ch;
                else
                    hor = false;
            } while (!(ch || cv));
            putShip(y, x, maxShipSize-i, hor);
        }
    }
    printField();
    state = ST_ATTACKING;
}

bool Field::attack(int x, int y)
{
    if (y<0 || x < 0)
        return false;
    if (field[y][x]<=maxShipSize)
    {
        if (field[y][x] == -1)
            field[y][x]++;
        field[y][x] += maxShipSize+1;
#ifdef DEBUG
        std::cout << checkKilledShip(x, y) << std::endl;
#endif
        killShip(x, y);
//        printField();
#ifndef DTM
        state = ST_ATTACKING;
        *es = ST_WAITING;
#endif
        std::cout << checkEndGame() << std::endl;
        return true;
    }
    return false;
}

void Field::autoAttack()
{
    int x, y;
    do
    {
        x = rand()%fieldSize;
        y = rand()%fieldSize;
    } while (!attack(x, y));

}

bool Field::isShipHorizontal(int x, int y)
{
    if (field[y][x] == 0 || field[y][x] == 1 || field[y][x] == -1 || field[y][x]%5 == 0)
        return false;
    if (x > 0 && field[y][x-1]%(maxShipSize+1) == field[y][x]%(maxShipSize+1))
        return true;
    if (x < fieldSize-1 && field[y][x+1]%(maxShipSize+1) == field[y][x]%(maxShipSize+1))
        return true;
    return false;
}

bool Field::checkKilledShip(int x, int y)
{
    //printField();
    if (field[y][x] <= maxShipSize+1)
        return false;
    if (field[y][x] == maxShipSize+2)
        return true;
    if (field[y][x] > 2*maxShipSize+2)
        return true;
    bool hor = isShipHorizontal(x, y);
    if (hor)
    {
        while (x<fieldSize-1 && field[y][x+1] != -1 && field[y][x+1]%5 != 0)
            ++x;
        for (int i = x; i>x-field[y][x]%(maxShipSize+1); --i)
        {
            if (field[y][i] <= maxShipSize)
                return false;
        }
    } else
    {
        while (y<fieldSize-1 && field[y+1][x] != -1 && field[y+1][x]%5 != 0)
            ++y;
        for (int i = y; i>y-field[y][x]%(maxShipSize+1); --i)
        {
            if (field[i][x] <= maxShipSize)
                return false;
        }
    }
    return true;
}

void Field::killShip(int x, int y)
{
    if (!checkKilledShip(x, y))
        return;
    bool hor = isShipHorizontal(x, y);
    if (hor)
    {
        while (x<fieldSize-1 && field[y][x+1] != -1 && field[y][x+1]%5 != 0)
            ++x;
        //printField();
        if (x<fieldSize-1)
        {
            field[y][x+1] = 10;
            if (y>0)
                field[y-1][x+1] = 10;
            if (y<fieldSize-1)
                field[y+1][x+1] = 10;
        }
        //printField();
        for (int i = x; i>x-field[y][x]%(maxShipSize+1); --i)
        {
            field[y][i] += maxShipSize+1;
            if (y>0)
                field[y-1][i] = 10;
            if (y<fieldSize-1)
                field[y+1][i] = 10;
        }
        printField();

        int fx = x-field[y][x]%(maxShipSize+1);
        if (fx>=0)
        {
            field[y][fx] = 10;
            if (y>0)
                field[y-1][fx] = 10;
            if (y<fieldSize-1)
                field[y+1][fx] = 10;
        }
        printField();

    } else
    {
        while (y<fieldSize-1 && field[y+1][x] != -1 && field[y+1][x]%5 != 0)
            ++y;
        if (y<fieldSize-1)
        {
            field[y+1][x] = 10;
            if (x>0)
                field[y+1][x-1] = 10;
            if (x<fieldSize-1)
                field[y+1][x+1] = 10;
        }
        printField();
        for (int i = y; i>y-field[y][x]%(maxShipSize+1); --i)
        {
            field[i][x] += maxShipSize+1;
            if (x>0)
                field[i][x-1] = 10;
            if (x<fieldSize-1)
                field[i][x+1] = 10;
        }
        printField();
        int fy = y-field[y][x]%(maxShipSize+1);
        if (fy>=0)
        {
            field[fy][x] = 10;
            if (x>0)
                field[fy][x-1] = 10;
            if (x<fieldSize-1)
                field[fy][x+1] = 10;
        }
        printField();
    }
}

bool Field::checkEndGame()
{
    bool res = true;
    for (int i = 0; i<fieldSize; ++i)
    {
        for (int j = 0; j<fieldSize; ++j)
        {
            res &= field[i][j] > maxShipSize || field[i][j] <= 0;
            std::cout << (field[i][j] > maxShipSize || field[i][j] <= 0);
        }
    }
    std::cout << std::endl;
    return res;
}
