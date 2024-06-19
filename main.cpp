#include <QCoreApplication>
#include <QTextStream>
#include "foldersizegrouper.h"
#include "typesizegrouper.h"
#include <stdexcept>
class Context
{
public:
    void setSizeGrouper(SizeGrouper* grouper)//Принимает указатель на SizeGrouper

    {
        _grouper.reset(grouper);//устанавливает новый объект
    }

    QMap<QString, qint64> makeGroup(const QString& path, qint64& total)
    {
        return _grouper ? _grouper->group(path, total) : QMap<QString, qint64>{};//Если объект не равен nullptr, то для него вызывается метод, который возвращает карту.
    }

private:
    std::unique_ptr<SizeGrouper> _grouper;
};



SizeGrouper* makeGrouper(const QString& mode)//принимает режим группировки и возваращает указатель
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

        qint64 total = 0;
        QMap<QString, qint64> group;
        try {
            ctx.setSizeGrouper(makeGrouper(mode));// создает экземпляр класса, который выбирается в соответствии с переданным режимом
            group = ctx.makeGroup(path, total); //метод создает группу файлов в указанном пути
        } catch (std::exception& ex) {//выбрасываются исключения
            cout << "Error: " << ex.what() << '\n';
            continue;
        }
        for (auto it = group.keyValueBegin(); it != group.keyValueEnd(); ++it) { //проходимся по всем элементам карты, пока не достигнем последнего элемента
            QString ssize = QString::number(it->second / 1024);//преобразуем размер группы в килобайты, а затем в строку
            QString spercent = "*";
            if (total != 0) {
                auto percent = 100 * static_cast<double>(it->second) / total;//вычисляем процентное соотношение размера текущей группы файлов относительно общего размера всех файлов
                spercent = percent > 0.01 ? QString::number(percent, 'f', 2) : "< 0.01";//Форматируем процент в строку с двумя знаками после запятой, если он больше 0.01
            }
           cout << it->first << " --- " << ssize << "kb --- " << spercent << "%\n";
        }

        cout << "---------------------------------\n";
    }

    return app.exec();
}
