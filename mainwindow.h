#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "wordslistwindow.h"
#include "trainingwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnDictionary_clicked();//обработчик открытия словаря
    void showMainWindowSlot();//слот показа главного окна
    void on_btnStart_clicked();//обработчик начала тренировки

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    WordsListWindow* wordListDialog;
    TrainingWindow* trainingDialog;
};

#endif // MAINWINDOW_H
