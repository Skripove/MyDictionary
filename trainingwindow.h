#ifndef TRAININGWINDOW_H
#define TRAININGWINDOW_H

#include <QDialog>

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
};

#endif // TRAININGWINDOW_H
