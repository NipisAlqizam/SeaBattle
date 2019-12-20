#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <time.h>
#include <vector>
#define DEBUG

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
#ifndef DEBUG
    srand(time(NULL));
    fh = new FieldWidget(ui->centralWidget, 20, 20, 350, true, rand());
    srand(rand());
    fc = new FieldWidget(ui->centralWidget, 450, 20, 350, false, rand());
#else
    int seed = time(NULL);
    fh = new FieldWidget(ui->centralWidget, 20, 20, 350, true, seed);
    fc = new FieldWidget(ui->centralWidget, 450, 20, 350, false, seed);
#endif
    setWindowTitle(QString("Морской бой"));
    fh->field.es = &fc->field.state;
    fc->field.es = &fh->field.state;
    connect(ui->action, SIGNAL(triggered(bool)), this, SLOT(newGame()));
    connect(ui->action_2, &QAction::triggered, this, &MainWindow::autoPlace);
    connect(fh, &FieldWidget::attacked, fc, &FieldWidget::doAttack);
    connect(fc, &FieldWidget::attacked, fh, &FieldWidget::doAttack);
    connect(fh, &FieldWidget::lost, this, &MainWindow::endGame);
    connect(fc, &FieldWidget::lost, this, &MainWindow::endGame);
    connect(fh, &FieldWidget::stateChanged, this, &MainWindow::updateState);
}

void MainWindow::autoPlace()
{
    fh->field.field = std::vector<std::vector<int>>(10, std::vector<int>(10, 0));
    fh->field.autoPlace();
    fc->field.state = ST_WAITING;
    fh->repaint();
    fc->repaint();
}

void MainWindow::newGame()
{
    fh->field.field = std::vector<std::vector<int>>(10, std::vector<int>(10, 0));
    fh->field.state = ST_PLACING_SHIPS;
    fh->setShip(0);
    fh->repaint();
    fc->field.field = std::vector<std::vector<int>>(10, std::vector<int>(10, 0));
    fc->field.autoPlace();
    fc->repaint();
}

QString MainWindow::endGameMessage(bool human)
{
    if (human) return QString("Игра окончена, вы проиграли.");
    return QString("Игра окончена, вы выиграли.");
}

void MainWindow::endGame(bool human)
{
    fh->field.state = ST_ENDGAME;
    fc->field.state = ST_ENDGAME;
    ui->stateLabel->setText(QString(endGameMessage(human)));
    if (human)
        ui->statusBar->showMessage("You've lost", 10000);
    else
        ui->statusBar->showMessage("You win", 10000);
}

void MainWindow::updateState(State new_state)
{
    if (new_state == State::ST_ATTACKING) ui->stateLabel->setText(QString("Стреляйте!"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
