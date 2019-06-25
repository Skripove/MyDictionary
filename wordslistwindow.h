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
    void showWordsCountSignal();//сигнал показа в MainWindow кол-ва доступных слов

private slots:
    void saveRuEngNumberSlot(QString, QString, int);//слот изменения и предоставления имени новой картинки
    void on_btnAddWord_clicked();//слот добавления нового слова
    void on_btnDelWord_clicked();//слот удаления выделенного слова
    void on_radioButtonRu_Eng_toggled(bool checked);//если включено, то отображаем Рус - Англ
    void on_radioButtonEng_Ru_toggled(bool checked);//если включено, то отображаем Англ - Рус
    void on_listWords_itemClicked(QListWidgetItem *item);//слот отображения картинки выделенного элемента
    void on_btnClose_clicked();//кнопка закрытия окна

private:
    void readDB();//Считывание БД
    void showList();//Отображаем список слов и картинку
    void addWordInDB(QString, QString, int);//добавление нового слова в БД и мапы, а так же отображение
    void addWordInMaps(QString, QString, int);//добавление нового слова в Мапы
    void deleteWord(int);//удалить слово
    void delWordFromDB(QString);//удалить слово из базы данных
    void delWordFromMaps(QString, QString);//удалить слово из Мапов
    void delImageFromDir(QString);//удалить картинку из папки
    void clearStat();//очистить статистику и картинку

private:
    Ui::WordsListWindow *ui;
    QMap<QString, QVector<QString>>* mapRU;
    QMap<QString, QVector<QString>>* mapENG;
    DialogAddWord* addWordDialog = nullptr;//указатель на диалоговое окно добавления
    int numberImg = 0;//номер последней картинки
    int wordsCount;//количество доступных слов
    QString fullDBName = QCoreApplication::applicationDirPath() + QDir::separator() + "db_name.sqlite";//адрес и имя БД

};

#endif // WORDSLISTWINDOW_H












