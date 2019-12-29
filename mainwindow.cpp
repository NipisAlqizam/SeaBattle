#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <time.h>
#include <vector>
#include <QPushButton>
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
    ui->size1Button->setCheckable(true);
    ui->size2Button->setCheckable(true);
    ui->size3Button->setCheckable(true);
    ui->size4Button->setCheckable(true);
    ui->size4Button->setChecked(true);
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
    connect(fh, &FieldWidget::shipFulled, this, &MainWindow::deactivateShipButton);
    connect(ui->size1Button, &QPushButton::clicked, this, &MainWindow::changeCurrentShipSize);
    connect(ui->size2Button, &QPushButton::clicked, this, &MainWindow::changeCurrentShipSize);
    connect(ui->size3Button, &QPushButton::clicked, this, &MainWindow::changeCurrentShipSize);
    connect(ui->size4Button, &QPushButton::clicked, this, &MainWindow::changeCurrentShipSize);
    connect(fh, &FieldWidget::shipSizeDecreased, this, &MainWindow::checkNewShipButton);
    connect(ui->rotateButton, &QPushButton::clicked, fh, &FieldWidget::rotateShip);

}

void MainWindow::autoPlace()
{
    if (fh->field.state != State::ST_PLACING_SHIPS) return;
    fh->field.field = std::vector<std::vector<int>>(10, std::vector<int>(10, 0));
    fh->field.autoPlace();
    fc->field.state = ST_WAITING;
    fh->repaint();
    fc->repaint();
    ui->stateLabel->setText(QString("Стреляйте!"));
    ui->size2Button->setEnabled(false);
    ui->size1Button->setEnabled(false);
    ui->size3Button->setEnabled(false);
    ui->size4Button->setEnabled(false);
    ui->size1Button->setChecked(true);
    ui->size2Button->setChecked(true);
    ui->size3Button->setChecked(true);
    ui->size4Button->setChecked(true);
}

void MainWindow::newGame()
{
    fh->field.field = std::vector<std::vector<int>>(10, std::vector<int>(10, 0));
    fh->field.state = ST_PLACING_SHIPS;
    fh->resetShips();
    fh->repaint();
    fc->field.field = std::vector<std::vector<int>>(10, std::vector<int>(10, 0));
    fc->field.autoPlace();
    fc->repaint();
    ui->stateLabel->setText(QString("Разместите корабли"));
    ui->size1Button->setEnabled(true);
    ui->size2Button->setEnabled(true);
    ui->size3Button->setEnabled(true);
    ui->size4Button->setEnabled(true);
    uncheckOtherShipButtons(ui->size4Button);
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
}

void MainWindow::updateState(State new_state)
{
    if (new_state == State::ST_ATTACKING) ui->stateLabel->setText(QString("Стреляйте!"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::deactivateShipButton(int shipSize)
{
    switch (shipSize)
    {
    case 4:
        ui->size4Button->setEnabled(false);
        break;
    case 3:
        ui->size3Button->setEnabled(false);
        break;
    case 2:
        ui->size2Button->setEnabled(false);
        break;
    case 1:
        ui->size1Button->setEnabled(false);
    }
}

void MainWindow::changeCurrentShipSize(bool checked)
{
    if (!checked)
        return;
    if (fh->field.state != State::ST_PLACING_SHIPS)
        return;
    uncheckOtherShipButtons(sender());
    int shipSize = -1;
    if (sender() == ui->size1Button) shipSize = 3;
    if (sender() == ui->size2Button) shipSize = 2;
    if (sender() == ui->size3Button) shipSize = 1;
    if (sender() == ui->size4Button) shipSize = 0;
    assert(shipSize != -1);
    fh->setShip(shipSize);
}

void MainWindow::checkNewShipButton(int shipSize)
{
    switch (shipSize) {
    case 0:
        ui->size4Button->setChecked(true);
        break;
    case 1:
        ui->size3Button->setChecked(true);
        break;
    case 2:
        ui->size2Button->setChecked(true);
        break;
    case 3:
        ui->size1Button->setChecked(true);
        break;
    }
}

void MainWindow::uncheckOtherShipButtons(QObject *current) {
    ui->size1Button->setChecked(false);
    ui->size2Button->setChecked(false);
    ui->size3Button->setChecked(false);
    ui->size4Button->setChecked(false);
    if (current == ui->size1Button) ui->size1Button->setChecked(true);
    if (current == ui->size2Button) ui->size2Button->setChecked(true);
    if (current == ui->size3Button) ui->size3Button->setChecked(true);
    if (current == ui->size4Button) ui->size4Button->setChecked(true);
}
