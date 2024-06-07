#include <QCoreApplication>
#include <QTextStream>
#include "foldersizegrouper.h"
#include "typesizegrouper.h"
#include <stdexcept>

class Context
{
public:
    void setSizeGrouper(SizeGrouper* grouper)
    {
        _grouper.reset(grouper);//устанавливает новый объект
    }

    QList<QString> makeGroup(const QString& path)//создаем список строк в заданном пути
    {
        if (!_grouper) {
            return {};
        }

        QList<QString> strreprs;//объявление списка строк
    //Заполнение списка строк
        qint64 total = 0;
        auto group = _grouper->group(path, total);//Вызываем метод group, который возвращает карту
        for (auto it = group.keyValueBegin(); it != group.keyValueEnd(); ++it) { //проходимся по всем элементам карты, пока не достигнем последнего элемента
            QString ssize = QString::number(it->second / 1024);//преобразуем размер группы в килобайты, а затем в строку
            QString spercent = 0;
            if (total != 0) {
                auto percent = 100 * static_cast<double>(it->second) / total;//вычисляем процентное соотношение размера текущей группы файлов относительно общего размера всех файлов
                spercent = percent > 0.01 ? QString::number(percent, 'f', 2) : "< 0.01";//Форматируем процент в строку с двумя знаками после запятой, если он больше 0.01
            }
            QString str = it->first + " --- " + ssize + "kb --- " + spercent + '%';//итоговая строка
            strreprs.append(str);//добавляет итоговую строку в список
        }

        return strreprs;
    }

private:
    std::unique_ptr<SizeGrouper> _grouper;
};


SizeGrouper* makeGrouper(const QString& mode)
{
    if (mode == "folders") {
        return new FolderSizeGrouper;//создат новый объект
    }
    if (mode == "types") {
        return new TypeSizeGrouper;
    }

    throw std::runtime_error("Unknown grouping mode when expected folders|types");//выбрасыват исключения

}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    Context ctx;

    while (true) {
        QTextStream cin(stdin);
        QTextStream cout(stdout);

        QString path = cin.readLine();//считывает строку из ввода, представляющая путь
        QString mode = cin.readLine();

        QList<QString> strlist;
        try {
            ctx.setSizeGrouper(makeGrouper(mode));// создает экземпляр класса, который выбирается в соответствии с переданным режимом
            strlist = ctx.makeGroup(path); //метод создает группу файлов в указанном пути
        } catch (std::exception& ex) {//выбрасываются исключения
            cout << "Error: " << ex.what() << '\n';
            continue;
        }

        for (const auto& x : strlist) {//проходит по каждому элементу в списке строк
            cout << x << '\n';
        }
        cout << "---------------------------------\n";
    }

    return app.exec();
}
