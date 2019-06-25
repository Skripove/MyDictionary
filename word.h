#ifndef WORD_H
#define WORD_H

#include <QString>

class Word
{
public:
    Word();//конструктор
    Word(QString, QString, QString, int, int, int);//конструктор от параметров
    ~Word(){}//деструктор

    QString getRu();//получить русское слово
    QString getEng();//получить английское слово
    QString getnameImg();//получить имя картинки
    int getShowing();//получить количество показов
    int getCorrectly();//получить количество правильных ответов
    int getRating();//получить рейтинг

    void addShowing();//увеличить показы
    void addCorrectly();//увеличить правильные ответы
    void addRating();//увеличить рейтинг
    void downRating();//уменьшить рейтинг

private:
    QString ru;//русское
    QString eng;//английское
    QString nameImg;//имя картинки
    int showing;//показы
    int correctly;//верные ответы
    int rating;//рейтинг
};

#endif // WORD_H
