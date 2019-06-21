#include "helpdialog.h"
#include "ui_helpdialog.h"

helpDialog::helpDialog(QWidget *parent) : QDialog(parent), ui(new Ui::helpDialog)
{
    ui->setupUi(this);
    ui->textEdit->setText(tr("<h4>Главное окно:</h4>"

                             "<p>На главном окне располагаются активные элементы: кнопки Начать, Словарь, Выход, "
                             "поле установки количества слов, радиокнопки выбора режима тренировки, а так же кнопки "
                             "О приложении и Справка (в левом верхнем углу). Число зеленого цвета показывает имеющееся количество слов в словаре.</p>"

                             "<p>\tКнопка \"Начать\" - запускает тренировочное окно. Перед началом необходимо указать количество желаемых слов и режим тренировки.</p>"
                             "<p>\tКнопка \"Словарь\" - открывает окно просмотра/добавления/удаления слов.</p>"
                             "<p>\tКнопка \"Выход\" - выходит из приложения.</p>"
                             "<p>\tПоле \"Количество слов\" - служит для ввода пользователем числа слов, которые будут по очереди отображаться в окне тренировки. "
                             "Принимает числа от 1 до имеющегося максимально доступного количества слов.</p>"
                             "<p>\tРадиокнопка \"Русский - ?\" - в тренировочном окне показывается русское слово, поле ответа принимает английское.</p>"
                             "<p>\tРадикнопка \"English - ?\" - в тренировочном окне показывается английское слово, поле ответа принимает русское.</p>"
                             "<p>\tКнопка \"О приложении\" - краткая информация о программе.</p>"
                             "<p>\tКнопка \"Справка\" - выводит данное окно со справочной информацией.</p>"

                             "<p></p>"

                             "<h4>Словарь:</h4>"

                             "<p>В словаре располагаются: поле со списком доступных слов, радиокнопки сортировки, поля отображения "
                             "картинки и информации о выделенном слове, кнопки добавления и удаления слова и кнопка Закрыть. "
                             "Число зеленого цвета показывает имеющееся количество слов в словаре.</p>"

                             "<p>\tРадиокнопка сортировки по \"Русским\" - сортирует список по русским словам (по алфавиту). "
                             "Отображается сначала русское слово, затем английское.</p>"
                             "<p>\tРадиокнопка сортировки по \"Ангийским\" - сортирует список по английским словам (по алфавиту). "
                             "Отображается сначала английское слово, затем русское.</p>"
                             "<p>\tКнопка \"+\" - открывает окно добавления новго слова.</p>"
                             "<p>\tКнопка \"-\" - удаляет выделенное слово.</p>"
                             "<p>\tКнопка \"Закрыть\" - закрывает окно словаря.</p>"

                             "<p></p>"

                             "<h4>Окно добавления нового слова:</h4>"

                             "<p>В окне имеются: поля для ввода русского и английского слов (каждое поле принимает только соответствующее ему знаение), "
                             "поле отображения картинки, ассоциирущейся со словом, кнопки Выбрать изображение и Готово.</p>"

                             "<p>\tКнопка \"Выбрать изображение\" - открывает окно выбора желаемого изображения. Если не загружать изображение, "
                             "то отображаться будет стандартное, с надписью \"Изображение отсутствует\".</p>"
                             "<p>\tКнопка \"Готово\" - сохраняет ввседенные слова с изображением и закрывает окно добавления нового слова.</p>"

                             "<p></p>"

                             "<h4>Окно тренировки:</h4>"

                             "<p>Отображает очередное слово и принимает его перевод. Имеет: поле отображения картинки (по необходимости), кнопки Пропуск и Ок.</p>"
                             "<p>\tПоле отображения картинки - показывает стандартное изображение с призывом \"Удерживать\" левую клавишу мыши на нем "
                             "для отображения скрытой картинки.</p>"
                             "<p>\tКнопка \"Пропуск\" - пропускает отображаемое слово и переходи к следующему (при его наличии). Ответ засчитывается как ошибочный.</p>"
                             "<p>\tКнопка \"Ок\" - принимает введенный ответ и переходит к следующему слову (при его наличии).</p>"
                             ));

}

helpDialog::~helpDialog()
{
    delete ui;
}
