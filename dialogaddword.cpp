#include <QFileDialog>//диалоговое окно выбора файла
#include <QDir>//Для поиска дирректории и разделителей
#include <QRegExp>
#include <QMessageBox>
#include "dialogaddword.h"
#include "ui_dialogaddword.h"

DialogAddWord::DialogAddWord(QWidget *parent) : QDialog(parent), ui(new Ui::DialogAddWord)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags () & (~Qt::WindowContextHelpButtonHint));//окно будет без вопростельного знака
    savedPic.load(":/res/img/noImg.png");//пустая картинка
    int w = ui->labelNewImage->width();//ширина лейбла новой картинки
    int h = ui->labelNewImage->height();//высота лейбла новой картинки
    ui->labelNewImage->setPixmap(savedPic.scaled(w, h, Qt::KeepAspectRatio));//положили пустую картинку в лейбл новой картинки
    if (savedPic == QPixmap(":/res/img/noImg.png"))
        ui->btnClearImage->hide();
    ui->lineAddRus->setFocus();
}

DialogAddWord::~DialogAddWord()
{
    delete ui;
}

void DialogAddWord::saveNewImageSlot(QString newName)//слот установки имени и сохранения новой картинки
{
    QString dir = QCoreApplication::applicationDirPath() + QDir::separator() + "saved_image";//адрес папки с картинками
    if(!QDir(dir).exists())//если папки с картинками не существует
        QDir().mkpath(dir);//то создать
    ui->labelNewImage->pixmap()->save(dir + QDir::separator() + newName + ".png", "png");//сохраняем картинку
}

void DialogAddWord::on_btnChangeImage_clicked()//Добавление своей картинки
{
    QString dirPath = QCoreApplication::applicationDirPath();//адрес открытия диалогового окна
    QString nameFile = QFileDialog::getOpenFileName(this,
           tr("Выбор изображения"),//Заголовок
          dirPath, //Настройка на нужную папку.
          tr("Файлы изображений (*.png *.jpg *.bmp)\n"
             "Изображения в формате PNG (*.png)\n"
             "Изображения в формате JPG (*.jpg)\n"
             "Изображения в формате BMP (*.bmp)"));
        if (nameFile.isEmpty()) return;//Файл не выбран
        savedPic.load(nameFile);//загружаем выбранную картинку в сохраняемое изображение
        int w = ui->labelNewImage->width();//ширина лейбла новой картинки
        int h = ui->labelNewImage->height();//высота лейбла новой картинки
        ui->labelNewImage->setPixmap(savedPic.scaled(w, h, Qt::KeepAspectRatio));//положили пустую картинку в лейбл новой картинки
        ui->btnClearImage->show();//показали форму
}

void DialogAddWord::on_btnClearImage_clicked()//обработка кнопки очистки изображения
{
    if(savedPic != QPixmap(":/res/img/noImg.png"))//если в сохраняемой картинке не лежит пустое изображение
    {
        savedPic.load(":/res/img/noImg.png");
        int w = ui->labelNewImage->width();//ширина лейбла картинки
        int h = ui->labelNewImage->height();//высота лейбла картинки
        ui->labelNewImage->setPixmap(savedPic.scaled(w, h, Qt::KeepAspectRatio));//положили пустую картинку в лейбл новой картинки
        ui->btnClearImage->hide();//скрыли форму
    }
}

void DialogAddWord::on_btnFinishAddWord_clicked()//обработка кнопки Готово
{
    if(ui->lineAddRus->text().isEmpty() || (ui->lineAddEng->text().isEmpty()))//если одна или другая строка в добавлении пуста
    {
        return;
    }

    QRegExp simbolsRu("^[[а-яА-ЯёЁ]+$");//регулярное выражение на проверку строки (только на русские символы)
    QRegExp simbolsEng("^[A-Za-z]+$");//регулярное выражение на проверку строки (только на английские символы)
    if(!simbolsRu.exactMatch(ui->lineAddRus->text()) || !simbolsEng.exactMatch(ui->lineAddEng->text()))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Введены не корректные данные");
        msgBox.setText("Слова должны соответствовать своим полям. Любые символы кроме русских и английских букв не допускаются.");
        msgBox.exec();
        return;
    }

    int name = 0;
    if(savedPic != QPixmap(":/res/img/noImg.png"))//если в сохраняемой картинке не лежит пустое изображение
    {
        name = 1;
    }

    emit getRuEngAddressSignal(ui->lineAddRus->text(), ui->lineAddEng->text(), name);//сигнал запроса имени новой картинки

    close();
}
