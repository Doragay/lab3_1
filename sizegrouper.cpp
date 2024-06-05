
#include "sizegrouper.h"
#include <QMap>
#include <QFileInfo>

QMap<QString, qint64> SizeGrouper::group(const QString& path, qint64& total)
{
    QMap<QString, qint64> groups;
    groupInto(path, groups, QFileInfo(path).fileName(), total);
    return groups;
}
