#include "FolderSizeGrouper.h"
#include <QDirIterator>//иттератор позволяющий проходить по данным внутри папки
#include <QFileInfo>
#include <stdexcept>
//генерирует группы проходясь в каждой папке по файлам, и потм вызвается рекурсивно для каждойй подпапки
void FolderSizeGrouper::groupInto(const QString& path, QMap<QString, qint64>& groups, const QString& currentDir, qint64& total)
{

    QFileInfo pathInfo(path);
    if (!pathInfo.exists() || !pathInfo.isDir()) {
        throw std::runtime_error("Directory \"" + path.toStdString() + "\" doesn't exists");
    }
    //Проходится рекурсивно по файлам. Рекурсивно= самовывоз функции
    QDirIterator fileIt(path, QDir::Files | QDir::NoSymLinks);//уровень вложенности,будет проходится. Засовываем путь+ флаги(проходимся по файлом или
    while (fileIt.hasNext()) { //пока у нас есть файл в каталоге
        auto info = QFileInfo(fileIt.next()); //перемщаем иттератор на следущий файл, и мы от текущего имени в каталоге, создам файл инфо
        groups[currentDir] += info.size();// в карту групп по текущей директории добавляем размер файла.
        total += info.size();
    }
    //создаем итератора, чтобы пройти по папкам в указанной директории.
    QDirIterator dirIt(path, QDir::Dirs | QDir::NoDotAndDotDot);//идём по папкам и
    while (dirIt.hasNext()) {
        auto info = QFileInfo(dirIt.next());//Получение информации о следующей папке
        groupInto(info.absoluteFilePath(), groups, currentDir + QDir::separator() +  info.fileName(), total);
        //Рекурсивно вызывает функцию groupInto для обработки этой папки, передавая полный путь, карту размеров групп и общий размер файлов.
    }
}
