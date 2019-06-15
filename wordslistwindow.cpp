#include "wordslistwindow.h"
#include "ui_wordslistwindow.h"
#include <QPixmap>
#include <QDir>
#include <QMessageBox>

WordsListWindow::WordsListWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WordsListWindow)
{
    ui->setupUi(this);
    mapRU = new QMap<QString, QVector<QString>>();
    mapENG = new QMap<QString, QVector<QString>>();
    dataBaseIsOK();//Проверка открытия или создание БД
    readDB();//Считывание БД
    showList();//Отображаем список слов
}

WordsListWindow::~WordsListWindow()
{
    delete ui;
    delete mapRU;
    delete mapENG;

}

bool WordsListWindow::dataBaseIsOK()//Проверка открытия или создание БД
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
        dbase.close();//закрыли БД
         return true;
}

bool WordsListWindow::readDB()//Считывание БД
{
    dbase.setDatabaseName(fullDBName);//утанавливаем адрес с именем базы
    if (!dbase.open())//если база не открывается
    {
          qDebug() << "Ошибка при открытии файла БД";
          return -1;
    }

    QSqlQuery obj_query;//объект для управления запросами
    if(!obj_query.exec("SELECT MAX(numberImg) FROM words_table;"))
        qDebug()<<"Не удалось открыть таблицу БД";
    obj_query.first();
    numberImg = obj_query.value(0).toInt();//прочитали номер самой последней картинки

    if(!obj_query.exec("SELECT ru, eng, numberImg FROM words_table;"))
        qDebug()<<"Не удалось открыть таблицу БД";
    QSqlRecord rec = obj_query.record();
    while(obj_query.next())
    {
        QVector<QString> tmpVecRU;//создали временный вектор RU
        QVector<QString> tmpVecENG;//создали временный вектор ENG

        tmpVecRU.push_back(obj_query.value(rec.indexOf("eng")).toString());//вставляем слова в вектор
        tmpVecRU.push_back(obj_query.value(rec.indexOf("numberImg")).toString());//вставляем слова в вектор

        tmpVecENG.push_back(obj_query.value(rec.indexOf("ru")).toString());//вставляем слова в вектор
        tmpVecENG.push_back(obj_query.value(rec.indexOf("numberImg")).toString());//вставляем слова в вектор

        mapRU->insert(obj_query.value(rec.indexOf("ru")).toString(), tmpVecRU);//заносим информацию в мапRU
        mapENG->insert(obj_query.value(rec.indexOf("eng")).toString(), tmpVecENG);//заносим информацию в мапENG
    }
    dbase.close();
    return true;
}

void WordsListWindow::showList()//Отображаем список слов
{
    QMap<QString, QVector<QString>>::iterator i;

    if(ui->radioButtonRu_Eng->isChecked())
    {
        ui->listWords->clear();//чистим список
        for (i = mapRU->begin(); i != mapRU->end(); ++i){
            ui->listWords->addItem(i.key() + " - " + i.value()[0]/* + i.value()[1]*/);
        }
    }
    else if(ui->radioButtonEng_Ru->isChecked())
    {
        ui->listWords->clear();//чистим список
        for (i = mapENG->begin(); i != mapENG->end(); ++i){
            ui->listWords->addItem(i.key() + " - " + i.value()[0]/* + i.value()[1]*/);
        }
    }
    //...добавить отображение картинки...
}

bool WordsListWindow::addWordInDB(QString ru, QString eng, int number)//добавление нового слова в БД
{
    dbase.setDatabaseName(fullDBName);//утанавливаем адрес с именем базы
    if (!dbase.open())//если база не открывается
    {
          qDebug() << "Ошибка при открытии файла БД";
          return -1;
    }

    QString newName = QString::number(number);//преобразуем число в строку
    QSqlQuery obj_query;//объект для управления запросами
    QString str = "INSERT INTO words_table VALUES('" + ru + "', '" + eng + "', "+ newName +");";

    if(!obj_query.exec(str))
        qDebug()<<"Не удалось занести строку в БД";
    dbase.close();
    return true;
}

bool WordsListWindow::addWordInMaps(QString ru, QString eng, int number)//добавление нового слова в мапы
{
    QString newName = QString::number(number);//преобразуем число в строку

    QVector<QString> tmpVecRU;//создали временный вектор RU
    QVector<QString> tmpVecENG;//создали временный вектор ENG

    tmpVecRU.push_back(eng);//вставляем слова в вектор
    tmpVecRU.push_back(newName);//вставляем слова в вектор

    tmpVecENG.push_back(ru);//вставляем слова в вектор
    tmpVecENG.push_back(newName);//вставляем слова в вектор

    mapRU->insert(ru, tmpVecRU);//заносим информацию в мапRU
    mapENG->insert(eng, tmpVecENG);//заносим информацию в мапENG

    return true;
}

void WordsListWindow::on_btnAddWord_clicked()//обработчик добавления нового слова
{
    addWordDialog = new DialogAddWord(this);//создали диалоговое окно для добавления новых слов
    addWordDialog->setAttribute(Qt::WA_DeleteOnClose);//очистка памяти при закрытии диалогового окна

    //соединение сигнала и слота сохранения новых слов и картинки с адресом
    connect(addWordDialog, SIGNAL(getRuEngAddressSignal(QString, QString, int)), this, SLOT (saveRuEngNumberSlot(QString, QString, int)));
    //соединение сигнала и слота предоставленияимени новой картинки
    connect(this, SIGNAL(provideNewImageNameSignal(QString)), addWordDialog, SLOT (saveNewImageSlot(QString)));
    addWordDialog->show();//показали форму
}

bool WordsListWindow::delWordFromDB(QString keyRu)//удалить слово из базы данных
{
    dbase.setDatabaseName(fullDBName);//утанавливаем адрес с именем базы
    if (!dbase.open())//если база не открывается
    {
          qDebug() << "Ошибка при открытии файла БД";
          return -1;
    }

    QSqlQuery obj_query;//объект для управления запросами
    QString str = "DELETE FROM words_table WHERE ru = '" + keyRu + "';";

    if(!obj_query.exec(str))
        qDebug()<<"Не удалось удалить строку из БД";
    dbase.close();
    return true;
}

void WordsListWindow::deleteWord()//удалить слово
{
    int itemInt;
    itemInt = ui->listWords->currentRow();//номер выбранной строки
    QString str = ui->listWords->item(itemInt)->text();//текст выбранной строки
    QStringList lst = str.split(" ");//Делим текст на слова через пробел
    QString keyRu;//ключ Ru
    QString keyEng;//ключ Eng
    if(ui->radioButtonRu_Eng->isChecked())
    {
        keyRu = lst.at(0);//ключ Ru - первая строка из lst
        keyEng = lst.at(2);//ключ Eng - третья строка из lst
    }

    else if(ui->radioButtonEng_Ru->isChecked())
    {
        keyRu = lst.at(2);//ключ Ru - третья строка из lst
        keyEng = lst.at(0);//ключ Eng - первая строка из lst
    }

    delWordFromDB(keyRu);//удалить слово из базы данных
    delWordFromMaps(keyRu, keyEng);//удалить слово из Мапов
    ui->listWords->takeItem(itemInt);//удалить слово из списка
    ui->labelImg->clear();
}

bool WordsListWindow::delWordFromMaps(QString keyRu, QString keyEng)//удалить слово из Мапов
{
    QString nameDelImage = mapRU->value(keyRu)[1];//имя удаляемой картинки
    mapRU->remove(keyRu);//удаляем значение из русского
    mapENG->remove(keyEng);//удаляем значение из английского
    if(nameDelImage != "0")
        delImageFromDir(nameDelImage);//удалить картинку из папки
    return true;
}

bool WordsListWindow::delImageFromDir(QString name)//удалить картинку из папки
{
    QString fullNImg = QCoreApplication::applicationDirPath() + QDir::separator() + "saved_image" +
            QDir::separator() + name + ".png";//полное имя файла с картинкой
    QFile file(fullNImg);
    file.remove();//удаляем файл картинки
    return true;
}

void WordsListWindow::on_btnDelWord_clicked()//обработчик кнопки удаления слова
{
    if(ui->listWords->count() <= 3)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Информация");
        msgBox.setText("Словарь должен содержать минимум 3 слова.");
        msgBox.exec();
        return;
    }
    QMessageBox msgBox;
    msgBox.setWindowTitle("Важно");
    msgBox.setText("Удалить слово без возможности восстановления?");
    //msgBox.setInformativeText("Ок - ДА\nCancel - ОТМЕНА");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int res = msgBox.exec();
    if (res == QMessageBox::Ok) //нажата кнопка Ok
        deleteWord();
    else //отмена
        return;
}

void WordsListWindow::saveRuEngNumberSlot(QString ru, QString eng, int name)//слот изменения и предоставления имени новой картинки
{
    if(name != 0)//если картинка новая, то создаем адрес с новым именем и отправляем сигналом
    {
        numberImg++;
        QString newName = QString::number(numberImg);//преобразуем число в строку
        emit provideNewImageNameSignal(newName);//сигнал передачи адреса и имени новой картинки
        name = numberImg;
    }
    addWordInDB(ru, eng, name);
    addWordInMaps(ru, eng, name);
    showList();
}

void WordsListWindow::on_listWords_itemClicked(QListWidgetItem *item)//слот отображения картинки выделенного элемента
{
    QString key = item->text();//берем текст из выделенного элемента
    QStringList lst = key.split(" ");//Делим строку на слова через пробел
    key = lst.at(0);//ключ - первая строка из lst
    QString imageName;
    if(ui->radioButtonRu_Eng->isChecked())
    {
       imageName = mapRU->value(key)[1];//значение последнего элемента (вектора по ключу)
    }

    else if(ui->radioButtonEng_Ru->isChecked())
    {
        imageName = mapENG->value(key)[1];//значение последнего элемента (вектора по ключу)
    }

    QString fullFN =":/res/img/noImg.png";

    if(imageName != "0")
    {
        fullFN = QCoreApplication::applicationDirPath() + QDir::separator() + "saved_image" +
                                          QDir::separator() + imageName + ".png";//полное имя файла с картинкой
    }

    QPixmap img(fullFN);
    int w = ui->labelImg->width();//ширина лейбла картинки
    int h = ui->labelImg->height();//высота лейбла картинки
    ui->labelImg->setPixmap(img.scaled(w, h, Qt::KeepAspectRatio));//положили картинку в лейбл
}

void WordsListWindow::on_radioButtonRu_Eng_toggled(bool checked)//отображаем Рус - Англ
{
    showList();
}

void WordsListWindow::on_radioButtonEng_Ru_toggled(bool checked)//отображаем Англ - Рус
{
    showList();
}


void WordsListWindow::on_pushButton_clicked()//кнопка закрытия окна
{
    showMainWindowSignal();//показать MainWindow
    close();
}
