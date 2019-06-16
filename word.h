#ifndef WORD_H
#define WORD_H

#include <QString>

class Word
{
public:
    Word();//конструктор
    Word(QString, QString, QString, int, int, int);//конструктор от параметров
    ~Word(){}//деструктор

    QString getRu();
    QString getEng();
    QString getnameImg();
    int getShowing();
    int getCorrectly();
    int getRating();

    void addShowing();
    void addCorrectly();
    void addRating();
    void downRating();

private:
    QString ru;
    QString eng;
    QString nameImg;
    int showing;
    int correctly;
    int rating;
};

#endif // WORD_H
