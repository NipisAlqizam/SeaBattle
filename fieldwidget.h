#ifndef FIELDWIDGET_H
#define FIELDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <utility>
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

private:
    bool horizontal;
    QPen mainPen, killedPen;
    int currentShipSize;
    std::pair<QPoint, QPoint> prevShip;
    void drawShip(int startX, int startY,bool killed, QPainter * painter, bool alpha = false);
    void drawMissed(int x, int y, QPainter * painter);
    void removePrevShip();
    int ship, sc;

protected:
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void paintEvent(QPaintEvent *event);

signals:
    void attacked();
    void lost(bool human);

public slots:
    void doAttack();

};

#endif // FIELDWIDGET_H
