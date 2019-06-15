#ifndef DIALOGADDWORD_H
#define DIALOGADDWORD_H

#include <QDialog>

namespace Ui {
class DialogAddWord;
}

class DialogAddWord : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddWord(QWidget *parent = nullptr);
    ~DialogAddWord();

signals:
    void getRuEngAddressSignal(QString, QString, int);//сигнал запроса имени новой картинки

private slots:
    void saveNewImageSlot(QString);//слот установки имени новой картинки

    void on_btnChangeImage_clicked();//добавить свою картинку
    void on_btnClearImage_clicked();//очистить добавленную картинку
    void on_btnFinishAddWord_clicked();//Готово


private:
    Ui::DialogAddWord *ui;
    QPixmap savedPic;//сохраняемая картинка
};

#endif // DIALOGADDWORD_H
