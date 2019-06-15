#ifndef WORDSLISTWINDOW_H
#define WORDSLISTWINDOW_H

#include <QDialog>
#include <QMap>
#include <QVector>
#include <QtSql>
#include <QListWidgetItem>
#include "dialogaddword.h"//Заголовочный диалогового окна добавления

namespace Ui {
class WordsListWindow;
}

class WordsListWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WordsListWindow(QWidget *parent = nullptr);
    ~WordsListWindow();

signals:
    void provideNewImageNameSignal(QString);//предоставить имя новой картинки
    void showMainWindowSignal();//сигнал показа MainWindow

private slots:
    void saveRuEngNumberSlot(QString, QString, int);//слот изменения и предоставления имени новой картинки

    void on_btnAddWord_clicked();//слот добавления нового слова
    void on_btnDelWord_clicked();//слот удаления выделенного слова

    void on_radioButtonRu_Eng_toggled(bool checked);//если включено, то отображаем Рус - Англ
    void on_radioButtonEng_Ru_toggled(bool checked);//если включено, то отображаем Англ - Рус

    void on_listWords_itemClicked(QListWidgetItem *item);//слот отображения картинки выделенного элемента

    void on_pushButton_clicked();//кнопка закрытия окна

private:
    bool dataBaseIsOK();//Проверка открытия или создание БД
    bool readDB();//Считывание БД
    void showList();//Отображаем список слов и картинку
    bool addWordInDB(QString, QString, int);//добавление нового слова в БД и мапы, а так же отображение
    bool addWordInMaps(QString, QString, int);//добавление нового слова в Мапы
    void deleteWord();//удалить слово
    bool delWordFromDB(QString);//удалить слово из базы данных
    bool delWordFromMaps(QString, QString);//удалить слово из Мапов
    bool delImageFromDir(QString);//удалить картинку из папки

private:
    Ui::WordsListWindow *ui;
    QMap<QString, QVector<QString>>* mapRU;
    QMap<QString, QVector<QString>>* mapENG;
    DialogAddWord* addWordDialog = nullptr;//указатель на диалоговое окно добавления
    int numberImg = 0;//номер последней картинки
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");//создали объект БД;
    QString fullDBName = QCoreApplication::applicationDirPath() + QDir::separator() + "db_name.sqlite";//адрес и имя БД
};

#endif // WORDSLISTWINDOW_H
