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
    void showWordsCountSlot();//слот показа кол-ва доступных слов
    void on_btnStart_clicked();//обработчик начала тренировки
    void on_btnExit_clicked();//кнопка выхода из программы

    void on_aboutApplication_triggered();//слот о приложении

    void on_actionhelp_triggered();

private:
    Ui::MainWindow *ui;
    //QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");//создали объект БД;
    QString fullDBName = QCoreApplication::applicationDirPath() + QDir::separator() + "db_name.sqlite";//адрес и имя БД
    int wordsCount;//количество доступных слов
    int userWordsCount;//желаемое количество слов

    WordsListWindow* wordListDialog;//указатель на словарь
    TrainingWindow* trainingDialog;//указатель на окно тренировки

    bool dataBaseIsOK();//Проверка открытия или создание БД и запись количества слов в ней
    bool showWordsCount();//показать кол-во доступных слов
    void animationShow();//анимация появления
};

#endif // MAINWINDOW_H
