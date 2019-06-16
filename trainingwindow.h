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

private:
    Ui::TrainingWindow *ui;
    int count;//количество показываемых слов
    QPixmap hoverImg;
    QList<Word*>* questionList;//указатель на список указателей на слова
    Word* intermediateWord;//указатель на слово-посредник

};

#endif // TRAININGWINDOW_H
