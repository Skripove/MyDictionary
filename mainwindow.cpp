#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataBaseIsOK();//Проверка открытия или создание БД и запись количества слов в ней
    ui->labelWordsCount->setText(QString::number(wordsCount));
}

MainWindow::~MainWindow()
{
    delete ui;
}
bool MainWindow::dataBaseIsOK()//Проверка открытия или создание БД и запись количества слов в ней
{
    //-------------SQLLite----------------
//        qDebug()<<fullDBName;
        QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");//создали объект БД;
        bool isDB_Open = false;//открыта или закрыта БД

        if(!QFile(fullDBName).exists())//если файла не существует
        {
            dbase.setDatabaseName(fullDBName);//утанавливаем адрес с именем базы
            if (!dbase.open())//база создаётся и если не открывается
            {
                  qDebug() << "Ошибка при создании + открытии БД";
                  return -1;
            }
            qDebug() << "Создали новую БД";
            QString str = "CREATE TABLE words_table (ru text, eng text, numberImg integer, showing integer, correctly integer, rating integer);";//строка запроса создания таблицы
            QSqlQuery obj_query;//объект для управления запросами
            bool q = obj_query.exec(str);//выполнился ли запрос?
            if(!q)
            {
                qDebug() << "Не создали таблицу";
                return -2;
            }
            qDebug() << "Добавили таблицу в БД";
            str = "INSERT INTO words_table VALUES ('кот', 'cat', 0, 0, 0, 0);";//строка запроса вставки строки
            q = obj_query.exec(str);//выполнился ли запрос?
            if(!q)
            {
                qDebug() << "Не внесли в таблицу строку";
                return -3;
            }
            isDB_Open = true;//база открыта
            qDebug() << "Внесли в таблицу строку";
        }

        if(!isDB_Open){//елси файл существует
            dbase.setDatabaseName(fullDBName);//утанавливаем адрес с именем базы
            if (!dbase.open())//если база не открывается
            {
                  qDebug() << "Ошибка при открытии файла БД";
                  return -1;
            }
            isDB_Open = true;//база открыта
            qDebug()<<"Нашли и открыли файл БД";
        }

        QSqlQuery obj_query;//объект для управления запросами
        if(!obj_query.exec("SELECT COUNT(numberImg) FROM words_table;"))
            qDebug()<<"Не удалось открыть таблицу БД";
        obj_query.first();
        wordsCount = obj_query.value(0).toInt();//записали количество слов в базе
        dbase.close();//закрыли БД
        return true;
}

void MainWindow::showWordsCountSlot()//слот показа кол-ва доступных слов
{
    showWordsCount();//показали кол-во доступных слов
}

bool MainWindow::showWordsCount()//показать кол-во доступных слов
{
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");//создали объект БД;
    dbase.setDatabaseName(fullDBName);//утанавливаем адрес с именем базы
    if (!dbase.open())//если база не открывается
    {
          qDebug() << "Ошибка при открытии файла БД";
          return -1;
    }
    QSqlQuery obj_query;//объект для управления запросами
    if(!obj_query.exec("SELECT COUNT(numberImg) FROM words_table;"))
        qDebug()<<"Не удалось открыть таблицу БД";
    obj_query.first();
    wordsCount = obj_query.value(0).toInt();//записали количество слов в базе
    dbase.close();//закрыли БД
    ui->labelWordsCount->setText(QString::number(wordsCount));
    return true;
}

void MainWindow::on_btnDictionary_clicked()//кнопка открытия словаря
{
    wordListDialog = new WordsListWindow(this);//создали диалоговое окно для добавления новых слов
    wordListDialog->setAttribute(Qt::WA_DeleteOnClose);//очистка памяти при закрытии диалогового окна
    //соединение сигнала и слота обновления кол-ва слов в главном окне
    connect(wordListDialog, SIGNAL(showWordsCountSignal()), this, SLOT (showWordsCountSlot()));
    //hide();//скрыли главное окно
    wordListDialog->show();//показали окно словаря
}

void MainWindow::on_btnStart_clicked()//слот запуска окна тренировок
{
    if(ui->lineWordCount->text().isEmpty())//проверка на ввод желаемого числа слов
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Информация");
        msgBox.setText("Введите желаемое количество слов (не более " + QString::number(wordsCount) + ")");
        msgBox.exec();
        return;
    }

    userWordsCount = ui->lineWordCount->text().toInt();//желаемое число слов

    if (userWordsCount > wordsCount)//проверка, чтобы введенное число не превышало кол-во доступных слов
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Информация");
        msgBox.setText("Число должно быть не больше " + QString::number(wordsCount));
        msgBox.exec();
        return;
    }

    trainingDialog = new TrainingWindow(this, wordsCount, userWordsCount);//создали диалоговое окно для тренировки
    trainingDialog->setAttribute(Qt::WA_DeleteOnClose);//очистка памяти при закрытии диалогового окна
    trainingDialog->show();
}

void MainWindow::on_btnExit_clicked()//слот выхода из программы
{
    qApp->quit();
}

