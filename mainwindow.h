#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "fieldwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    FieldWidget * fh, * fc;
    ~MainWindow();


public slots:
    void newGame();
    void autoPlace();
    void endGame(bool human);
    void updateState(State new_state);
    void changeCurrentShipSize(bool checked);
    void deactivateShipButton(int shipSize);
    void checkNewShipButton(int shipSize);
    void updateShipsCount();
    void about();
    void help();

private:
    Ui::MainWindow *ui;
    QString endGameMessage(bool human);
    void uncheckOtherShipButtons(QObject *current);
    QString shipsCountString;
};

#endif // MAINWINDOW_H
