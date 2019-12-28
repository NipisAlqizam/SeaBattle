#ifndef FIELDWIDGET_H
#define FIELDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <utility>
#include <vector>
#include "field.h"

class FieldWidget : public QWidget
{
    Q_OBJECT
    int cellSize, cellCount, fieldSize, fieldStart;
public:
    Field field;
    bool human;
    explicit FieldWidget(QWidget *parent = nullptr, int x = 0, int y = 0, int sideSize = 200, bool human = true, int seed = 0);
    QPoint getCell(int x, int y);
    State *enemyState;
    void resetShips();

private:
    bool horizontal;
    QPen mainPen, killedPen;
    int currentShipSize;
    std::pair<QPoint, QPoint> prevShip;
    void drawShip(int startX, int startY,bool killed, QPainter * painter, bool alpha = false);
    void drawMissed(int x, int y, QPainter * painter);
    void removePrevShip();
    int ship, shipCount;
    std::vector<int> shipsCount;
    /* ships_count[i] - количество i-палубных кораблей,
     * размещённых в данный момент на поле.
     */
    void placeShip(QPoint cell);

protected:
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void paintEvent(QPaintEvent *event);

signals:
    void attacked();
    void lost(bool human);
    void stateChanged(State new_state);
    void shipFulled(int shipSize);

public slots:
    void doAttack();

};

#endif // FIELDWIDGET_H
