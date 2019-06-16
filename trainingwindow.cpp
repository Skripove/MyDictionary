#include "trainingwindow.h"
#include "ui_trainingwindow.h"

TrainingWindow::TrainingWindow(QWidget *parent) : QDialog(parent), ui(new Ui::TrainingWindow)
{
    ui->setupUi(this);
    hoverImg.load(":/res/img/hoverIMG.png");
    int w = ui->labelImg->width();//ширина лейбла картинки
    int h = ui->labelImg->height();//высота лейбла картинки
    ui->labelImg->setPixmap(hoverImg.scaled(w, h, Qt::KeepAspectRatio));//положили главную картинку в лейбл
}

TrainingWindow::~TrainingWindow()
{
    delete ui;
    delete questionList;
    delete intermediateWord;
}

void TrainingWindow::setCount(int c)//установить количество показываемых слов
{
    this->count = c;
}
