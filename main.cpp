#include <QCoreApplication>
#include <QTextStream>
#include <iostream>
#include "foldersizegrouper.h"
#include "typesizegrouper.h"
#include <stdexcept>

//группирует файлы в зависимости от указанного режима группировки
QMap<QString, qint64> groupFileSizes(const QString& path, const QString& mode, qint64& total)
{
    QMap<QString, qint64> sizes;
    if (mode == "folders") {
        sizes = FolderSizeGrouper().group(path, total);
    } else if (mode == "types") {
        sizes = TypeSizeGrouper().group(path, total);
    }
    return sizes;
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    while (true) {
        QTextStream cin(stdin);
        QTextStream cout(stdout);

        QString path = cin.readLine();
        QString mode = cin.readLine();

        qint64 total = 0;
        QMap<QString, qint64> group;

        try {
            group = groupFileSizes(path, mode, total);
        } catch (std::exception& ex) {
            cout << "Error: " << ex.what() << '\n';
            continue;
        }

        for (auto it = group.keyValueBegin(); it != group.keyValueEnd(); ++it) {
            QString ssize = QString::number(it->second / 1024);
            QString spercent = "*";
            if (total != 0) {
                auto percent = 100 * static_cast<double>(it->second) / total;
                spercent = percent > 0.01 ? QString::number(percent, 'f', 2) : "< 0.01";
            }
            cout << it->first << " --- " << ssize << "kb --- " << spercent << "%\n";
        }
        cout << "---------------------------------\n";
    }

    return app.exec();
}
