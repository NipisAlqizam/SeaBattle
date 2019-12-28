#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void changeCurrentShipSize();
    void deactivateFulledShipButton(int shipSize);

private:
    Ui::MainWindow *ui;
    QString endGameMessage(bool human);
};

#endif // MAINWINDOW_H
