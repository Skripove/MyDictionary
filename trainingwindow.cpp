#include <QDebug>
#include <iterator>
#include <QMessageBox>
#include <QTimer>
#include <QCloseEvent>
#include "trainingwindow.h"
#include "ui_trainingwindow.h"

TrainingWindow::TrainingWindow(QWidget *parent, int c, int u, bool rn) : QDialog(parent), ui(new Ui::TrainingWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags () & (~Qt::WindowContextHelpButtonHint));//окно будет без вопростельного знака
    ru_eng = rn;//вариант тестирования
    count = c;//количество слов
    userWordsCount = u;//задаем количество желаемых слов от параметра "u"
    currentNumWord = 0;//номер текущего слова
    questionList = new QList<Word*>();
    clickImg.load(":/res/img/hoverIMG.png");
    answerTrue.load(":/res/img/answerT.png");
    answerFalse.load(":/res/img/answerF.png");
    ui->btnExit->hide();//скрыли картику выхода
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
}

void TrainingWindow::readToList()//считывание слов из БД
{
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");//создали объект БД;
    dbase.setDatabaseName(fullDBName);//утанавливаем адрес с именем базы
    if (!dbase.open())//если база не открывается
    {
          qDebug() << "Ошибка при открытии файла БД";
    }
    read70FromDB();//считываем 70%
    read30FromDB();//считываем 30%
    dbase.close();
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
        int c = obj_query.value(rec.indexOf("correctly")).toInt();
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
        int c = obj_query.value(rec.indexOf("correctly")).toInt();
        int r = obj_query.value(rec.indexOf("rating")).toInt();

        intermediateWord = new Word(ru, eng, n, s, c, r);//создали указатель на объект слова
        questionList->push_back(intermediateWord);//добавили указатель на слово в список
    }
}

void TrainingWindow::startTest()//запуск тестирования
{
    it = questionList->begin();//устанавливаем итератор на начало списка
}

void TrainingWindow::showWord()//отобразить слово и установаить скрытую картинку
{
    ui->lineAnswer->clear();//очистили поле ввода

    if(ru_eng) ui->labelTest->setText((*it)->getRu());//отображаем тестируемое русское слово
    else ui->labelTest->setText((*it)->getEng());//отображаем тестируемое английское слово

    if((*it)->getnameImg()=="0"){
        hiddenImg.load(":/res/img/noImg.png");//если имя картинки "0", то загружаем пустую
    }
    else {
        hiddenImg.load(dir + QDir::separator() + (*it)->getnameImg() + ".png");//иначе загружаем из папки
    }

    setStyleSheet("#TrainingWindow{background-color: white;}");
    ui->lineAnswer->show();//показали поле для ответа
    ui->lineAnswer->setFocus();//установили фокус на поле ввода
    ui->labelTrueFalse->clear();//чистим индикатор ответа
    ui->btnOk->show();//показали кнопку ОК
    ui->btnSkip->show();//показали кнопку Пропуск

    currentNumWord++;
    ui->labelNumWord->setText(QString::number(currentNumWord) + " из " + QString::number(userWordsCount));//отображение номера текущего слова из userWordsCount слов
}

void TrainingWindow::on_btnShowImg_pressed()//обработчик удержания кнопки
{
    //ui->labelImg->setStyleSheet("#labelImg{background-color: white;}");//белый фон
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

void TrainingWindow::on_btnOk_clicked()//обработчик кнопки ОК
{
    if(ui->lineAnswer->text().isEmpty())//проверка на пустой ответ
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Осечка");
        msgBox.setText("Введите ответ. Желательно правильный.");
        msgBox.exec();
        ui->lineAnswer->setFocus();//установили фокус на поле ввода
        return;
    }

    ui->lineAnswer->hide();//скрыли поле для ответа
    ui->btnOk->hide();//скрыли кнопку ОК
    ui->btnSkip->hide();//скрыли кнопку Пропуск

    if(ru_eng)//если тест рус-англ
    {
        if(ui->lineAnswer->text().toLower() == (*it)->getEng().toLower())//проверка на правильный ответ рус-англ
            answerCorrectly();//действия при верном ответе
        else
            answerWrong();//действия при НЕверном ответе
    }

    else//если тест англ-рус
    {
        if(ui->lineAnswer->text().toLower() == (*it)->getRu().toLower())//проверка на правильный ответ англ-рус
            answerCorrectly();//действия при верном ответе
        else
            answerWrong();//действия при НЕверном ответе
    }

    QTimer::singleShot(1000, this, SLOT(nextOrShowResult()));//по таймеру следующее слово или выход
}

void TrainingWindow::answerCorrectly()//действия при верном ответе
{
    int w = ui->labelTrueFalse->width();//ширина лейбла картинки
    int h = ui->labelTrueFalse->height();//высота лейбла картинки

    ui->labelTrueFalse->setPixmap(answerTrue.scaled(w, h, Qt::KeepAspectRatio));//индикатор - верно
    setStyleSheet("#TrainingWindow{background-color: rgb(217,255,189);}");//зеленый фон
    cor++;//увеличили количество верных ответов
    saveCorrectly();//верный ответ
}

void TrainingWindow::answerWrong()//действия при НЕверном ответе
{
    int w = ui->labelTrueFalse->width();//ширина лейбла картинки
    int h = ui->labelTrueFalse->height();//высота лейбла картинки

    ui->labelTrueFalse->setPixmap(answerFalse.scaled(w, h, Qt::KeepAspectRatio));//индикатор -НЕ верно
    setStyleSheet("#TrainingWindow{background-color: rgb(255,187,188);}");//красный фон
    wr++;//увелеичили количество Не верных ответов
    saveWrong();//НЕ верный ответ
}

void TrainingWindow::saveCorrectly()//сохранили верный ответ
{
    qDebug()<<"Ответ верный";
    (*it)->addShowing();
    (*it)->addCorrectly();
    (*it)->addRating();

    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");//создали объект БД;
    dbase.setDatabaseName(fullDBName);//утанавливаем адрес с именем базы
    if (!dbase.open())//если база не открывается
    {
          qDebug() << "Ошибка при открытии файла БД";
    }
    QString ru = (*it)->getRu();
    QString eng = (*it)->getEng();
    QString s = QString::number((*it)->getShowing());//преобразуем число в строку
    QString c = QString::number((*it)->getCorrectly());//преобразуем число в строку
    QString r = QString::number((*it)->getRating());//преобразуем число в строку

    QSqlQuery obj_query;//объект для управления запросами
    QString str = "UPDATE words_table SET showing = " + s + ", correctly = " + c + ", rating = " + r + " WHERE ru = '" + ru + "' AND eng = '" + eng + "';";

    if(!obj_query.exec(str))
        qDebug()<<"Не удалось изменить статистику БД";
    dbase.close();
}

void TrainingWindow::saveWrong()//сохранили неверный ответ
{
    qDebug()<<"Ответ НЕ верный";
    (*it)->addShowing();
    (*it)->downRating();

    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");//создали объект БД;
    dbase.setDatabaseName(fullDBName);//утанавливаем адрес с именем базы
    if (!dbase.open())//если база не открывается
    {
          qDebug() << "Ошибка при открытии файла БД";
    }
    QString ru = (*it)->getRu();
    QString eng = (*it)->getEng();
    QString s = QString::number((*it)->getShowing());//преобразуем число в строку
    QString r = QString::number((*it)->getRating());//преобразуем число в строку

    QSqlQuery obj_query;//объект для управления запросами
    QString str = "UPDATE words_table SET showing = " + s + ", rating = " + r + " WHERE ru = '" + ru + "' AND eng = '" + eng + "';";

    if(!obj_query.exec(str))
        qDebug()<<"Не удалось изменить статистику БД";
    dbase.close();
}

void TrainingWindow::on_btnSkip_clicked()//обработчик кнопки пропуск
{
    ui->lineAnswer->hide();//скрыли поле для ответа
    ui->btnOk->hide();//скрыли кнопку ОК
    ui->btnSkip->hide();//скрыли кнопку Пропуск

    int w = ui->labelTrueFalse->width();//ширина лейбла картинки
    int h = ui->labelTrueFalse->height();//высота лейбла картинки
    ui->labelTrueFalse->setPixmap(answerFalse.scaled(w, h, Qt::KeepAspectRatio));//индикатор -НЕ верно
    setStyleSheet("#TrainingWindow{background-color: rgb(255,187,188);}");//красный фон
    skip++;//увеличили количество пропусков
    saveWrong();//засчитывается как не верный ответ
    qDebug()<<"Пропустили";

    QTimer::singleShot(1000, this, SLOT(nextOrShowResult()));//по таймеру следующее слово или выход
}

void TrainingWindow::nextOrShowResult()//слот следующего слова или выхода
{
    it++;//переходим на следующее слово
    if(it == questionList->end())//если следующего слова нет
    {
        qDebug()<<"Закончили";
        showResult();//отображаем результат
        return;
    }
    showWord();//отображаем слово
}

void TrainingWindow::showResult()//показать результат
{
    setStyleSheet("#TrainingWindow{background-color: white;}");//делаем фон белым

    //скрываем лишние элементы
    ui->labelTest->hide();
    ui->btnShowImg->hide();
    ui->lineAnswer->hide();
    ui->labelNumWord->hide();
    ui->labelTrueFalse->hide();
    ui->btnSkip->hide();
    ui->btnOk->hide();

    //чистим центральный лейбл и отображаем в нем резульат
    ui->labelImg->clear();
    ui->labelImg->setStyleSheet("#labelImg{color: rgb(80, 80, 80); background-color: rgba(255,234,189, 255); border-radius: 150px;}");//фон с ораньжевым кругом
    ui->labelImg->setText("Количество верных ответов: " + QString::number(cor) + "\n" +
                          "Количество ошибок: " + QString::number(wr) + "\n" +
                          "Количество пропусков: " + QString::number(skip));
    ui->btnExit->show();//показали кнопку выхода
}

void TrainingWindow::on_btnExit_clicked()//обработчик кнопки выхода
{
    close();
}

void TrainingWindow::closeEvent(QCloseEvent *event)//событие закрытия окна тренировки
{
    showMainWindowSignal();//сигнал показа главного окна
    event->accept();   //подтверждаем событие - добросовестно закрываем приложение
}


