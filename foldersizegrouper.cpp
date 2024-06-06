#include "foldersizegrouper.h"
#include <QDirIterator>
#include <QFileInfo>
#include <stdexcept>

void FolderSizeGrouper::groupInto(const QString& path, QMap<QString, qint64>& groups, const QString& currentDir, qint64& total)
{
    QFileInfo pathInfo(path);
    if (!pathInfo.exists() || !pathInfo.isDir()) {
        throw std::runtime_error("Directory \"" + path.toStdString() + "\" doesn't exists");
    }
//Проходится по всем файлам и суммирует их размеры
    qint64 currentTotal = 0;
    QDirIterator fileIt(path, QDir::Files | QDir::NoSymLinks);
    while (fileIt.hasNext()) { //пока у нас есть файл в каталоге
        auto info = QFileInfo(fileIt.next());
        currentTotal += info.size();
    }
//создаем итератора, чтобы пройти по папкам исключая специальные директории
    QDirIterator dirIt(path, QDir::Dirs | QDir::NoDotAndDotDot);
    while (dirIt.hasNext()) {
        auto info = QFileInfo(dirIt.next());
        groupInto(info.absoluteFilePath(), groups, currentDir + QDir::separator() +  info.fileName(), currentTotal);
    }

    total += currentTotal;
    groups[currentDir] = currentTotal;
}
