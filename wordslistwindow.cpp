#include "wordslistwindow.h"
#include "ui_wordslistwindow.h"
#include <QPixmap>
#include <QDir>
#include <QMessageBox>
#include <QMouseEvent>

WordsListWindow::WordsListWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WordsListWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint /*| Qt::FramelessWindowHint*/);//окно будет без кнопок
    mapRU = new QMap<QString, QVector<QString>>();
    mapENG = new QMap<QString, QVector<QString>>();
    readDB();//Считывание БД
    showList();//Отображаем список слов
    ui->labelWordsCount->setText(QString::number(wordsCount));
}

WordsListWindow::~WordsListWindow()
{
    delete ui;
    delete mapRU;
    delete mapENG;
}

bool WordsListWindow::readDB()//Считывание БД
{
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");//создали объект БД;
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

    if(!obj_query.exec("SELECT ru, eng, numberImg, showing, correctly, rating FROM words_table;"))
        qDebug()<<"Не удалось открыть таблицу БД";
    QSqlRecord rec = obj_query.record();
    while(obj_query.next())
    {
        QVector<QString> tmpVecRU;//создали временный вектор RU
        QVector<QString> tmpVecENG;//создали временный вектор ENG

        tmpVecRU.push_back(obj_query.value(rec.indexOf("eng")).toString());//вставляем слова в вектор
        tmpVecRU.push_back(obj_query.value(rec.indexOf("numberImg")).toString());//вставляем слова в вектор

        tmpVecRU.push_back(obj_query.value(rec.indexOf("showing")).toString());//вставляем статистику
        tmpVecRU.push_back(obj_query.value(rec.indexOf("correctly")).toString());//вставляем статистику
        tmpVecRU.push_back(obj_query.value(rec.indexOf("rating")).toString());//вставляем статистику

        tmpVecENG.push_back(obj_query.value(rec.indexOf("ru")).toString());//вставляем слова в вектор
        tmpVecENG.push_back(obj_query.value(rec.indexOf("numberImg")).toString());//вставляем слова в вектор

        tmpVecENG.push_back(obj_query.value(rec.indexOf("showing")).toString());//вставляем статистику
        tmpVecENG.push_back(obj_query.value(rec.indexOf("correctly")).toString());//вставляем статистику
        tmpVecENG.push_back(obj_query.value(rec.indexOf("rating")).toString());//вставляем статистику

        mapRU->insert(obj_query.value(rec.indexOf("ru")).toString(), tmpVecRU);//заносим информацию в мапRU
        mapENG->insert(obj_query.value(rec.indexOf("eng")).toString(), tmpVecENG);//заносим информацию в мапENG
    }
    dbase.close();
    wordsCount = mapRU->count();//запоминаем количество слов
    return true;
}

void WordsListWindow::showList()//Отображаем список слов
{
    QMap<QString, QVector<QString>>::iterator i;

    if(ui->radioButtonRu_Eng->isChecked())
    {
        ui->listWords->clear();//чистим список
        for (i = mapRU->begin(); i != mapRU->end(); ++i){
            ui->listWords->addItem(i.key() + " - " + i.value()[0]);
        }
    }
    else if(ui->radioButtonEng_Ru->isChecked())
    {
        ui->listWords->clear();//чистим список
        for (i = mapENG->begin(); i != mapENG->end(); ++i){
            ui->listWords->addItem(i.key() + " - " + i.value()[0]);
        }
    }
}

bool WordsListWindow::addWordInDB(QString ru, QString eng, int number)//добавление нового слова в БД
{
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");//создали объект БД;
    dbase.setDatabaseName(fullDBName);//утанавливаем адрес с именем базы
    if (!dbase.open())//если база не открывается
    {
          qDebug() << "Ошибка при открытии файла БД";
          return -1;
    }

    QString newName = QString::number(number);//преобразуем число в строку
    QSqlQuery obj_query;//объект для управления запросами
    QString str = "INSERT INTO words_table VALUES('" + ru + "', '" + eng + "', "+ newName +", 0, 0, 0);";

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

    tmpVecRU.push_back("0");//вставляем статистику
    tmpVecRU.push_back("0");//вставляем статистику
    tmpVecRU.push_back("0");//вставляем статистику

    tmpVecENG.push_back(ru);//вставляем слова в вектор
    tmpVecENG.push_back(newName);//вставляем слова в вектор

    tmpVecENG.push_back("0");//вставляем статистику
    tmpVecENG.push_back("0");//вставляем статистику
    tmpVecENG.push_back("0");//вставляем статистику

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
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");//создали объект БД;
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

void WordsListWindow::deleteWord(int itemInt)//удалить слово
{
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
    clearStat();//очищаем картинку и статистику
    wordsCount--;
    ui->labelWordsCount->setText(QString::number(wordsCount));
}

void WordsListWindow::clearStat()//очистить статистику и картинку
{
    ui->labelImg->clear();//очистить картинку
    ui->labelShowing->clear();//очистить статистику
    ui->labelCorrectly->clear();//очистить статистику
    ui->labelRating->clear();//очистить статистику
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
    int itemInt = -1;
    itemInt = ui->listWords->currentRow();//номер выбранной строки
    if(itemInt == -1) return;

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
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int res = msgBox.exec();
    if (res == QMessageBox::Ok) //нажата кнопка Ok
        deleteWord(itemInt);
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
    clearStat();//очищаем картинку и статистику
    wordsCount++;
    ui->labelWordsCount->setText(QString::number(wordsCount));
    showList();
}

void WordsListWindow::on_listWords_itemClicked(QListWidgetItem *item)//слот отображения картинки выделенного элемента и его статистики
{
    QString key = item->text();//берем текст из выделенного элемента
    QStringList lst = key.split(" ");//Делим строку на слова через пробел
    key = lst.at(0);//ключ - первая строка из lst
    QString imageName;
    if(ui->radioButtonRu_Eng->isChecked())
    {
       imageName = mapRU->value(key)[1];//значение последнего элемента (вектора по ключу)
       ui->labelShowing->setText("Показы: " + mapRU->value(key)[2]);
       ui->labelCorrectly->setText("Правильные ответы: " + mapRU->value(key)[3]);
       ui->labelRating->setText("Рейтинг: " + mapRU->value(key)[4]);
    }

    else if(ui->radioButtonEng_Ru->isChecked())
    {
        imageName = mapENG->value(key)[1];//значение последнего элемента (вектора по ключу)
        ui->labelShowing->setText("Показы: " + mapENG->value(key)[2]);
        ui->labelCorrectly->setText("Правильные ответы: " + mapENG->value(key)[3]);
        ui->labelRating->setText("Рейтинг: " + mapENG->value(key)[4]);
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
    clearStat();
    showList();
}

void WordsListWindow::on_radioButtonEng_Ru_toggled(bool checked)//отображаем Англ - Рус
{
    clearStat();
    showList();
}

void WordsListWindow::on_btnClose_clicked()//кнопка закрытия окна
{
    emit showWordsCountSignal();//сигнал показа в MainWindow кол-ва доступных слов
    close();
}



//Перемещение окна мышкой (глюченый)
//void WordsListWindow::mousePressEvent(QMouseEvent *event)
//{
//    QRect p = this->geometry();
//    if(event->button() == Qt::LeftButton)
//    {
//        lastPoint = event->pos();
//        if(!p.contains(lastPoint))
//            return;
//        b_move = true;
//    }
//}

//void WordsListWindow::mouseMoveEvent(QMouseEvent *event)
//{
//    if((event->buttons() & Qt::LeftButton) && b_move)
//        move(event->globalX()-lastPoint.x(),
//             event->globalY()-lastPoint.y());
//}

//void WordsListWindow::mouseReleaseEvent(QMouseEvent *event)
//{
//    if (event->button() == Qt::LeftButton && b_move) {
//            b_move = false;
//        }
//}
