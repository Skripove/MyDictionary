#include "word.h"

Word::Word()
{

}

Word::Word(QString ru, QString eng, QString n, int s, int c, int r)//конструктор от параметров
{
    this->ru = ru;
    this->eng = eng;
    this->nameImg = n;
    this->showing = s;
    this->correctly = c;
    this->rating = r;
}

QString Word::getRu(){QString tmp = this->ru; return tmp;}
QString Word::getEng(){QString tmp = this->eng; return tmp;}
QString Word::getnameImg(){QString tmp = this->nameImg; return tmp;}
int Word::getShowing(){int tmp = this->showing; return tmp;}
int Word::getCorrectly(){int tmp = this->correctly; return tmp;}
int Word::getRating(){int tmp = this->rating; return tmp;}

void Word::addShowing() {this->showing++;}
void Word::addCorrectly() {this->correctly++;}
void Word::addRating(){this->rating++;}
void Word::downRating(){ if(this->rating != 0) this->rating--;}
