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
    explicit TrainingWindow(QWidget *parent = nullptr, int c = 0, int u = 0, bool rn = true);
    ~TrainingWindow();

private slots:

    void on_btnShowImg_pressed();//обработчик удержания кнопки
    void on_btnShowImg_released();//обработчик отпускания кнопки показа изображения
    void on_btnOk_clicked();//обработчик кнопки ОК

    void on_btnSkip_clicked();

private:
    Ui::TrainingWindow *ui;
    bool ru_eng;//рус-англ тест
    int count;//количество показываемых слов
    double userWordsCount;//желаемое количество слов
    int currentNumWord;//номер текущего тестируемого слова
    int seventy;//70%
    int thirty;//30%
    QString fullDBName = QCoreApplication::applicationDirPath() + QDir::separator() + "db_name.sqlite";//адрес и имя БД
    QPixmap clickImg;//картинка с указанием кликнуть
    QPixmap hiddenImg;//скрытая картинка
    QString dir = QCoreApplication::applicationDirPath() + QDir::separator() + "saved_image";//адрес папки с картинками
    QList<Word*>* questionList;//указатель на список указателей на слова
    QList<Word*>::iterator it;//итератор списка указателей на слова
    Word* intermediateWord;//указатель на слово-посредник


    void calcPercentWords();//вычисление процентов слов
    bool readToList();//считывание из БД в список
    void read70FromDB();//считывание необходимых 70% слов из БД
    void read30FromDB();//считывание необходимых 30% слов из БД
    void startTest();//запуск тестирования
    void showWord();//отобразить слово

    bool answerCorrectly();//верный ответ
    bool answerWrong();//неверный ответ

};

//Количество слов, заданное пользователем делится на 70% и 30%. 70% - запрос с сортировкой
//по рейтингу,  кол-ву показов и количеству правильных ответов. Остальные 30% - рандомные строки
//у которых столбец ru не совпадает ни с одним ru из 70%

#endif // TRAININGWINDOW_H
