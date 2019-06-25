#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainToolBar->addAction(ui->aboutApplication);
    ui->mainToolBar->addAction(ui->actionhelp);
    dataBaseIsOK();//Проверка открытия или создание БД и запись количества слов в ней
    ui->labelWordsCount->setText(QString::number(wordsCount));
    ui->lineWordCount->setFocus();//фокус на ввод кол-ва слов
    animationShow();//плавное появление главного окна
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::animationShow()//анимация появления
{
    setWindowOpacity(0);
    QPropertyAnimation* animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(300);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
}

void MainWindow::dataBaseIsOK()//Проверка открытия или создание БД и запись количества слов в ней
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
            }
            qDebug() << "Создали новую БД";
            QString str = "CREATE TABLE words_table (ru text, eng text, numberImg integer, showing integer, correctly integer, rating integer);";//строка запроса создания таблицы
            QSqlQuery obj_query;//объект для управления запросами
            bool q = obj_query.exec(str);//выполнился ли запрос?
            if(!q)
            {
                qDebug() << "Не создали таблицу";
            }
            qDebug() << "Добавили таблицу в БД";
            str = "INSERT INTO words_table VALUES ('кот', 'cat', 0, 0, 0, 0);";//строка запроса вставки строки
            q = obj_query.exec(str);//выполнился ли запрос?
            if(!q)
            {
                qDebug() << "Не внесли в таблицу строку";
            }
            isDB_Open = true;//база открыта
            qDebug() << "Внесли в таблицу строку";
        }

        if(!isDB_Open){//елси файл существует
            dbase.setDatabaseName(fullDBName);//утанавливаем адрес с именем базы
            if (!dbase.open())//если база не открывается
            {
                  qDebug() << "Ошибка при открытии файла БД";
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
}

void MainWindow::showWordsCountSlot()//слот показа кол-ва доступных слов и отображения главного окна
{
    showWordsCount();//показали кол-во доступных слов
    show();//показали окно
}

void MainWindow::showWordsCount()//показать кол-во доступных слов
{
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");//создали объект БД;
    dbase.setDatabaseName(fullDBName);//утанавливаем адрес с именем базы
    if (!dbase.open())//если база не открывается
    {
          qDebug() << "Ошибка при открытии файла БД";
    }
    QSqlQuery obj_query;//объект для управления запросами
    if(!obj_query.exec("SELECT COUNT(numberImg) FROM words_table;"))
        qDebug()<<"Не удалось открыть таблицу БД";
    obj_query.first();
    wordsCount = obj_query.value(0).toInt();//записали количество слов в базе
    dbase.close();//закрыли БД
    ui->labelWordsCount->setText(QString::number(wordsCount));
}

void MainWindow::on_btnDictionary_clicked()//кнопка открытия словаря
{
    wordListDialog = new WordsListWindow();//создали диалоговое окно для добавления новых слов
    wordListDialog->setAttribute(Qt::WA_DeleteOnClose);//очистка памяти при закрытии диалогового окна
    //соединение сигнала и слота обновления кол-ва слов в главном окне и показа главного окна
    connect(wordListDialog, SIGNAL(showWordsCountSignal()), this, SLOT (showWordsCountSlot()));
    wordListDialog->show();//показали окно словаря
    hide();//скрыли главное окно
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

    QRegExp num("^[0-9]+$");//регулярное выражение на проверку на ЧИСЛО желаемых слов
    if(!num.exactMatch(ui->lineWordCount->text()))
    {
        ui->lineWordCount->clear();
        return;
    }

    if(ui->lineWordCount->text().toInt() <= 0)//проверка на ввод желаемого числа слов
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Информация");
        msgBox.setText("Ха-ха-ха... Очень смешно.");
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

    bool ru_eng = true;//по умолчанию рус-англ тест
    if(ui->radioEng_Ru->isChecked()) ru_eng = false;//если стоит другой флаг, то англ-рус тест

    trainingDialog = new TrainingWindow(nullptr, wordsCount, userWordsCount, ru_eng);//создали диалоговое окно для тренировки
    trainingDialog->setAttribute(Qt::WA_DeleteOnClose);//очистка памяти при закрытии диалогового окна
    //соединение сигнала и слота обновления кол-ва слов в главном окне и показа главного окна
    connect(trainingDialog, SIGNAL(showMainWindowSignal()), this, SLOT (showWordsCountSlot()));
    trainingDialog->show();//показали тренировочное окно
    hide();//скрыли главное окно
}

void MainWindow::on_btnExit_clicked()//слот выхода из программы
{
    qApp->quit();
}


void MainWindow::on_aboutApplication_triggered()//слот о приложении
{
    QMessageBox::about(this, tr("О приложении Мой словарь"),
                       tr("<h2>Мой словарь 1.0</h2>"
                          "<p>Приложение \"Мой словарь\" предназначено для "
                          "тренировки английских слов.</p>"
                          "<p>Отличительными особенностями программы являются "
                          "возможность добавления изображений, ассоциирующихся со словами, а так же автоматическое распределение слов "
                          "по успеваемости пользователя. Новые и плохо изученные слова всплывают чаще, но иногда будут "
                          "появляться и хорошо отработанные, для дополнительного контроля.</p>"
                          "<p></p>"
                          "<p>Разработчик: Скрипов Егор Владимирович</p>"));
}

void MainWindow::on_actionhelp_triggered()//обработчик кнопки Справка
{
    helpDialog* help = new helpDialog(this);
    help->setAttribute(Qt::WA_DeleteOnClose);//очистка памяти при закрытии диалогового окна
    help->show();
}
















