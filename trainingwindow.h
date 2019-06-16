#ifndef TRAININGWINDOW_H
#define TRAININGWINDOW_H

#include <QDialog>
#include "word.h"

namespace Ui {
class TrainingWindow;
}

class TrainingWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TrainingWindow(QWidget *parent = nullptr);
    ~TrainingWindow();
    void setCount(int);//установить количество показываемых слов


private:
    Ui::TrainingWindow *ui;
    int count;//количество показываемых слов
    QPixmap hoverImg;//картинка с указанием кликнуть
    QList<Word*>* questionList;//указатель на список указателей на слова
    Word* intermediateWord;//указатель на слово-посредник

    void beginFillList();//начать заполнение списка


};

#endif // TRAININGWINDOW_H
