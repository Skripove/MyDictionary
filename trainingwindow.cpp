#include <QDebug>
#include <iterator>
#include "trainingwindow.h"
#include "ui_trainingwindow.h"

TrainingWindow::TrainingWindow(QWidget *parent, int c, int u) : QDialog(parent), ui(new Ui::TrainingWindow)
{
    ui->setupUi(this);    
    count = c;//количество слов
    userWordsCount = u;//задаем количество желаемых слов от параметра "u"
    questionList = new QList<Word*>();
    clickImg.load(":/res/img/hoverIMG.png");
    int w = ui->labelImg->width();//ширина лейбла картинки
    int h = ui->labelImg->height();//высота лейбла картинки
    ui->labelImg->setPixmap(clickImg.scaled(w, h, Qt::KeepAspectRatio));//положили главную картинку в лейбл

    calcPercentWords();//вычисление процентов слов
    readToList();//считываем желаемое кол-во слов в список для тренировки
    startTest();
    showWord();
}

TrainingWindow::~TrainingWindow()
{
    delete ui;

    QList<Word*>::iterator i;//итератором бежим по списку указателей на Word(ы) и удаляем каждый
    for(i = questionList->begin(); i!=questionList->end(); ++i){
        delete *i;
    }

    delete questionList;//удаляем список
}

void TrainingWindow::calcPercentWords()//вычисление процентов слов
{
    seventy = (userWordsCount/100)*70;//70%
    thirty = userWordsCount - seventy;//30%
    //qDebug()<<seventy;
    //qDebug()<<thirty;
}

bool TrainingWindow::readToList()//считывание необходимых 70% слов из БД
{
    dbase.setDatabaseName(fullDBName);//утанавливаем адрес с именем базы
    if (!dbase.open())//если база не открывается
    {
          qDebug() << "Ошибка при открытии файла БД";
          return -1;
    }
    read70FromDB();//считываем 70%
    read30FromDB();//считываем 30%
    dbase.close();
    return true;
}

void TrainingWindow::read70FromDB()//считывание необходимых 70% слов из БД
{
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
}

void TrainingWindow::read30FromDB()//считывание необходимых 30% слов из БД
{
    QString thit = QString::number(userWordsCount - seventy);//оставшиеся 30% в строку
    QStringList tmp;//список для проверки по ru
    QList<Word*>::iterator i;
    for(i = questionList->begin(); i!=questionList->end(); ++i){
        tmp.push_back("'" + (*i)->getRu() + "'");//вставляем в tmp русские слова из questionList
    }
    QString s = tmp.join(", ");//преобразовали в строку с разделителями

    QSqlQuery obj_query;//объект для управления запросами
    QString str = "SELECT ru, eng, numberImg, showing, correctly, rating FROM words_table WHERE ru NOT IN ("
                                                                        + s + ") ORDER BY RANDOM() LIMIT " + thit + ";";
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
}

void TrainingWindow::startTest()//запуск тестирования
{
    it = questionList->begin();//устанавливаем итератор на начало списка
}

void TrainingWindow::showWord()//отобразить слово
{
    ui->labelTest->setText((*it)->getRu());//отображаем тестируемое слово

    if((*it)->getnameImg()=="0"){
        hiddenImg.load(":/res/img/noImg.png");//если имя картинки "0", то загружаем пустую
    }
    else {
        hiddenImg.load(dir + QDir::separator() + (*it)->getnameImg() + ".png");//иначе загружаем из папки
    }
}


void TrainingWindow::on_btnShowImg_pressed()//обработчик удержания кнопки
{
    int w = ui->labelImg->width();//ширина лейбла картинки
    int h = ui->labelImg->height();//высота лейбла картинки
    ui->labelImg->setPixmap(hiddenImg.scaled(w, h, Qt::KeepAspectRatio));//показали скрытую картинку
}

void TrainingWindow::on_btnShowImg_released()//обработчик отпускания кнопки показа изображения
{
    int w = ui->labelImg->width();//ширина лейбла картинки
    int h = ui->labelImg->height();//высота лейбла картинки
    ui->labelImg->setPixmap(clickImg.scaled(w, h, Qt::KeepAspectRatio));//картинка с призывом нажать
}
