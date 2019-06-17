#ifndef TRAININGWINDOW_H
#define TRAININGWINDOW_H

#include <QtSql>
#include <QDialog>
#include "word.h"

namespace Ui {
class TrainingWindow;
}

class TrainingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TrainingWindow(QWidget *parent = nullptr, int c = 0);
    ~TrainingWindow();

private:
    Ui::TrainingWindow *ui;
    double count;//количество показываемых слов
    int seventy;//70%
    int thirty;//30%
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");//создали объект БД;
    QString fullDBName = QCoreApplication::applicationDirPath() + QDir::separator() + "db_name.sqlite";//адрес и имя БД
    QPixmap clickImg;//картинка с указанием кликнуть
    QList<Word*>* questionList;//указатель на список указателей на слова
    //Word* intermediateWord;//указатель на слово-посредник

    void beginFillList();//начать заполнение списка
    void calcPercentWords();//вычисление процентов слов
    bool read70FromDB();//считывание необходимых 70% слов из БД


};

#endif // TRAININGWINDOW_H
