#include "trainingwindow.h"
#include "ui_trainingwindow.h"

TrainingWindow::TrainingWindow(QWidget *parent) : QDialog(parent), ui(new Ui::TrainingWindow)
{
    ui->setupUi(this);
}

TrainingWindow::~TrainingWindow()
{
    delete ui;
}
