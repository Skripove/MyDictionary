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
    read70FromDB();
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
    QSqlQuery obj_query;//объект для управления запросами
    QString sev = QString::number(seventy);//перевели в строку
    QString str = "SELECT ru, eng, numberImg, showing, correctly, rating FROM words_table ORDER BY rating, showing, correctly LIMIT " + sev + ";";
    if(!obj_query.exec(str))
        qDebug()<<"Не удалось открыть таблицу БД";
    QSqlRecord rec = obj_query.record();
    while(obj_query.next())
    {
        QString ru = obj_query.value(rec.indexOf("ru")).toString();
        QString eng = obj_query.value(rec.indexOf("eng")).toString();
        QString n = obj_query.value(rec.indexOf("numberImg")).toString();
        int s = obj_query.value(rec.indexOf("showing")).toInt();
        int c = obj_query.value(rec.indexOf("currectly")).toInt();
        int r = obj_query.value(rec.indexOf("rating")).toInt();

        intermediateWord = new Word(ru, eng, n, s, c, r);//создали указатель на объект слова
        questionList->push_back(intermediateWord);//добавили указатель на слово в список
    }
    dbase.close();
    return true;
}

















