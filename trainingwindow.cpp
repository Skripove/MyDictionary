#include <QDebug>
#include "trainingwindow.h"
#include "ui_trainingwindow.h"

TrainingWindow::TrainingWindow(QWidget *parent, int c) : QDialog(parent), ui(new Ui::TrainingWindow)
{
    ui->setupUi(this);
    count = c;//задаем количество желаемых слов от параметра "с"
    questionList = new QList<Word*>();
    clickImg.load(":/res/img/hoverIMG.png");
    int w = ui->labelImg->width();//ширина лейбла картинки
    int h = ui->labelImg->height();//высота лейбла картинки
    ui->labelImg->setPixmap(clickImg.scaled(w, h, Qt::KeepAspectRatio));//положили главную картинку в лейбл

    calcPercentWords();//вычисление процентов слов
}

TrainingWindow::~TrainingWindow()
{
    delete ui;
    delete questionList;
    //delete intermediateWord;
}

void TrainingWindow::calcPercentWords()//вычисление процентов слов
{
    seventy = (count/100)*70;//70%
    thirty = count - seventy;//30%
    //qDebug()<<seventy;
    //qDebug()<<thirty;
}

bool TrainingWindow::read70FromDB()//считывание необходимых 70% слов из БД
{
    dbase.setDatabaseName(fullDBName);//утанавливаем адрес с именем базы
    if (!dbase.open())//если база не открывается
    {
          qDebug() << "Ошибка при открытии файла БД";
          return -1;
    }

//    QSqlQuery obj_query;//объект для управления запросами
//    if(!obj_query.exec("SELECT ru, eng, numberImg FROM words_table;"))
//        qDebug()<<"Не удалось открыть таблицу БД";
//    QSqlRecord rec = obj_query.record();
//    while(obj_query.next())
//    {
//        QVector<QString> tmpVecRU;//создали временный вектор RU
//        QVector<QString> tmpVecENG;//создали временный вектор ENG

//        tmpVecRU.push_back(obj_query.value(rec.indexOf("eng")).toString());//вставляем слова в вектор
//        tmpVecRU.push_back(obj_query.value(rec.indexOf("numberImg")).toString());//вставляем слова в вектор

//        tmpVecENG.push_back(obj_query.value(rec.indexOf("ru")).toString());//вставляем слова в вектор
//        tmpVecENG.push_back(obj_query.value(rec.indexOf("numberImg")).toString());//вставляем слова в вектор

//        mapRU->insert(obj_query.value(rec.indexOf("ru")).toString(), tmpVecRU);//заносим информацию в мапRU
//        mapENG->insert(obj_query.value(rec.indexOf("eng")).toString(), tmpVecENG);//заносим информацию в мапENG
//    }
    dbase.close();
    return true;
}

















