#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnDictionary_clicked()
{
    wordListDialog = new WordsListWindow(this);//создали диалоговое окно для добавления новых слов
    wordListDialog->setAttribute(Qt::WA_DeleteOnClose);//очистка памяти при закрытии диалогового окна
    //соединение сигнала и слота показа главного окна
    connect(wordListDialog, SIGNAL(showMainWindowSignal()), this, SLOT (showMainWindowSlot()));
    hide();//скрыли главное окно
    wordListDialog->show();//показали окно словаря
}

void MainWindow::showMainWindowSlot()//слот показа главного окна
{
    show();
}

void MainWindow::on_btnStart_clicked()
{
    trainingDialog = new TrainingWindow(this);//создали диалоговое окно для тренировки
    trainingDialog->setAttribute(Qt::WA_DeleteOnClose);//очистка памяти при закрытии диалогового окна
    trainingDialog->show();
}
