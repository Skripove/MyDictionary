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
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");//создали объект БД;
    QString fullDBName = QCoreApplication::applicationDirPath() + QDir::separator() + "db_name.sqlite";//адрес и имя БД
    int wordsCount;//количество доступных слов

    WordsListWindow* wordListDialog;//указатель на словарь
    TrainingWindow* trainingDialog;//указатель на окно тренировки

    bool dataBaseIsOK();//Проверка открытия или создание БД и запись количества слов в ней
};

#endif // MAINWINDOW_H
