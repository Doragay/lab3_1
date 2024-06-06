#include <iostream>
#include <QTextStream>
#include "foldersizegrouper.h"
#include "typesizegrouper.h"

enum class GroupingMode
{
    Folders,
    Types
};

QMap<QString, qint64> groupFileSizes(const QString& path, GroupingMode mode, qint64& total)
{
    QMap<QString, qint64> sizes;
    if (mode == GroupingMode::Folders) {
        sizes = FolderSizeGrouper().group(path, total);
    } else {
        sizes = TypeSizeGrouper().group(path, total);
    }
    return sizes;
}

int main()
{
    QTextStream in(stdin);
    QTextStream out(stdout);

    QString path = in.readLine();

    qint64 total = 0;
    QMap<QString, qint64> group = groupFileSizes(path, GroupingMode::Folders, total);

    for (auto it = group.keyValueBegin(); it != group.keyValueEnd(); ++it) {
        QString spercent = "*";
        if (total != 0) {
            auto percent = 100 * static_cast<double>(it->second) / total;
            spercent = percent > 0.01 ? QString::number(percent, 'f', 2) : "< 0.01";
        }
        out << it->first << " --- " << spercent << "%\n";
    }
    return 0;

}
