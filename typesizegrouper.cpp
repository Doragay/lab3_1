#include "typesizegrouper.h"
#include <QDirIterator>
#include <stdexcept>

void TypeSizeGrouper::groupInto(const QString& path, QMap<QString, qint64>& groups, const QString& currentDir, qint64& total)
{
    Q_UNUSED(currentDir);
    QFileInfo pathInfo(path);
    if (!pathInfo.exists() || !pathInfo.isDir()) {
        throw std::runtime_error("Directory \"" + path.toStdString() + "\" doesn't exists");
    }

    QDirIterator fileIt(path, QDir::Files | QDir::NoSymLinks);
    while (fileIt.hasNext()) {
        auto info = QFileInfo(fileIt.next()); //получает информацию о следущем файле
        groups["*." + info.suffix()] += info.size();
        total += info.size();
    }

    QDirIterator dirIt(path, QDir::Dirs | QDir::NoDotAndDotDot);
    while (dirIt.hasNext()) {
        auto info = QFileInfo(dirIt.next());
        groupInto(info.absoluteFilePath(), groups, currentDir + QDir::separator() +  info.fileName(), total);
    }
}
