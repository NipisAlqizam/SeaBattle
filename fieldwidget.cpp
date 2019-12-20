#include "fieldwidget.h"
#include <QPen>
#include <QDebug>
#include <QColor>
#include <QString>
#include <QChar>
#include <QPoint>


FieldWidget::FieldWidget(QWidget *parent, int x, int y, int sideSize, bool human, int seed)
    : QWidget(parent),
      cellCount(10),
      fieldSize(sideSize-sideSize%cellCount),
      field(10, seed),
      human(human),
      currentShipSize(4)
{
    horizontal = true;
    qDebug() << cellCount << " " << (sideSize-sideSize%cellCount);
    qDebug() << field.fieldSize;
    fieldSize = (sideSize-sideSize%cellCount);
    this->cellSize = sideSize/cellCount;
    this->setGeometry(x, y, sideSize+cellSize, sideSize+cellSize);
    this->fieldStart = cellSize;
    ship = 0;
    sc = 0;
    if (!human)
    {
        field.state = ST_WAITING;
        field.autoPlace();
    }
//    setMouseTracking(true);

}

void FieldWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setBrush(QColor(19,179,171));
    painter.setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap));
    painter.drawRect(fieldStart, fieldStart, fieldSize, fieldSize);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    for (int i = fieldStart; i<=width(); i+=this->cellSize)
    {
        painter.drawLine(i, fieldStart, i, fieldStart+fieldSize);
        painter.drawLine(fieldStart, i, fieldStart+fieldSize, i);
    }
    for (int i = fieldStart; i<width(); i+=cellSize)
    {
        painter.drawText(i, 0, cellSize, cellSize, Qt::AlignCenter|Qt::AlignVCenter, QString(L'А'+(i-fieldStart)/cellSize));
        painter.drawText(0, i, cellSize, cellSize, Qt::AlignCenter|Qt::AlignVCenter, QString::number(i/cellSize));
    }

    for (int i = 0; i<cellCount; ++i)
    {
        for (int j = 0; j<cellCount; ++j)
        {
            if (field.field[i][j] == 10 || field.field[i][j] == field.maxShipSize+1)
                drawMissed(j+1, i+1, &painter);
            else if (human && field.field[i][j] > 0 && field.field[i][j] <= field.maxShipSize)
                drawShip(j+1, i+1, false, &painter, false);
            else if (field.field[i][j] > field.maxShipSize+1)
                drawShip(j+1, i+1, true, &painter, false);
        }
    }
}

void FieldWidget::drawShip(int startX, int startY, bool killed, QPainter *painter, bool alpha)
{
    if (alpha)
    {
        mainPen = QPen(QColor(0, 0, 0, 50), 2, Qt::SolidLine, Qt::FlatCap);
        killedPen = QPen(QColor(255, 0, 0, 50), 2, Qt::SolidLine, Qt::FlatCap);
    }
    else
    {
        mainPen = QPen(Qt::black, 2, Qt::SolidLine, Qt::FlatCap);
        killedPen = QPen(Qt::red, 2, Qt::SolidLine, Qt::FlatCap);
    }
    if (killed)
        painter->setPen(killedPen);
    else
        painter->setPen(mainPen);
    painter->drawLine(startX*cellSize, startY*cellSize, (startX+1)*cellSize, (startY+1)*cellSize);
    painter->drawLine((startX+1)*cellSize, startY*cellSize, startX*cellSize, (startY+1)*cellSize);
}

void FieldWidget::drawMissed(int x, int y, QPainter *painter)
{
    painter->setPen(QPen(Qt::blue));
    painter->setBrush(QBrush(Qt::blue));
    painter->drawEllipse(QPoint(x*cellSize+cellSize/2, y*cellSize+cellSize/2), cellSize/4, cellSize/4);
}

QPoint FieldWidget::getCell(int x, int y)
{
    return QPoint((int) x/cellSize-1, (int) y/cellSize-1);
}

void FieldWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint cell = getCell(event->pos().rx(), event->pos().ry());
    if (human && field.state == State::ST_PLACING_SHIPS && field.checkShip(cell.y(), cell.x(), currentShipSize, horizontal))
    {

    }
}

void FieldWidget:: mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        horizontal = !horizontal;
        return;
    }
    QPoint cell = getCell(event->pos().x(), event->pos().y());
    if (human && field.state == State::ST_PLACING_SHIPS && field.checkShip(cell.y(), cell.x(), currentShipSize, horizontal))
    {
        currentShipSize = field.maxShipSize-ship;
        field.putShip(cell.y(), cell.x(), currentShipSize, horizontal);
        sc++;
        if (sc >= ship+1)
        {
            sc = 0;
            ship++;
            if (ship >= field.maxShipSize)
            {
                sc = 0;
                ship = 0;
                field.state = ST_ATTACKING;
                *field.es = ST_WAITING;
                emit stateChanged(ST_ATTACKING);
            }
        }
    }
    else if (!human && field.state == State::ST_WAITING)
    {
        field.attack(cell.x(), cell.y());
        if (field.checkEndGame())
            emit lost(human);
//        qDebug() << human << " player has attacked, state is" << field.state;
        emit attacked();
    }
//    qDebug() << human << ' ' << field.state;
    repaint();
}

void FieldWidget::doAttack()
{
    if (human && field.state == ST_WAITING)
    {
        field.autoAttack();
        if (field.checkEndGame())
            emit lost(human);
        repaint();
//        qDebug() << "autoattacked, state is" << field.state;
    }
}

void FieldWidget::setShip(int n)
{
    ship = n;
}


