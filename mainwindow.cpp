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

void MainWindow::on_btnDictionary_clicked()
{
    wordListDialog = new WordsListWindow(this);//создали диалоговое окно для добавления новых слов
    wordListDialog->setAttribute(Qt::WA_DeleteOnClose);//очистка памяти при закрытии диалогового окна
    //соединение сигнала и слота показа главного окна
    connect(wordListDialog, SIGNAL(showMainWindowSignal()), this, SLOT (showMainWindowSlot()));
    //hide();//скрыли главное окно

    wordListDialog->show();//показали окно словаря
}

void MainWindow::showMainWindowSlot()//слот показа главного окна
{
    show();
}

void MainWindow::on_btnStart_clicked()//слот запуска окна тренировок
{
    trainingDialog = new TrainingWindow(this);//создали диалоговое окно для тренировки
    trainingDialog->setAttribute(Qt::WA_DeleteOnClose);//очистка памяти при закрытии диалогового окна
    trainingDialog->show();
}

void MainWindow::on_pushButton_clicked()//слот выхода из программы
{
    qApp->quit();
}

bool MainWindow::dataBaseIsOK()//Проверка открытия или создание БД и запись количества слов в ней
{
    //-------------SQLLite----------------
//        qDebug()<<fullDBName;
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
            QString str = "CREATE TABLE words_table (ru text, eng text, numberImg integer);";//строка запроса создания таблицы
            QSqlQuery obj_query;//объект для управления запросами
            bool q = obj_query.exec(str);//выполнился ли запрос?
            if(!q)
            {
                qDebug() << "Не создали таблицу";
                return -2;
            }
            qDebug() << "Добавили таблицу в БД";
            str = "INSERT INTO words_table VALUES ('кот', 'cat', 0);";//строка запроса вставки строки
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
