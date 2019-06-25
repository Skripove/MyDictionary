#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "wordslistwindow.h"
#include "trainingwindow.h"
#include "helpdialog.h"

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
    void showWordsCountSlot();//слот показа кол-ва доступных слов и отображения главного окна
    void on_btnStart_clicked();//обработчик начала тренировки
    void on_btnExit_clicked();//кнопка выхода из программы

    void on_aboutApplication_triggered();//слот О приложении

    void on_actionhelp_triggered();//слот Справка

private:
    Ui::MainWindow *ui;
    QString fullDBName = QCoreApplication::applicationDirPath() + QDir::separator() + "db_name.sqlite";//адрес и имя БД
    int wordsCount;//количество доступных слов
    int userWordsCount;//желаемое количество слов

    WordsListWindow* wordListDialog;//указатель на словарь
    TrainingWindow* trainingDialog;//указатель на окно тренировки

    void dataBaseIsOK();//Проверка открытия или создание БД и запись количества слов в ней
    void showWordsCount();//показать кол-во доступных слов
    void animationShow();//анимация появления
};

#endif // MAINWINDOW_H
