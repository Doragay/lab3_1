#include "sizegrouper.h"
#include <QFileInfo>

SizeGrouper::~SizeGrouper()
{
}

QMap<QString, qint64> SizeGrouper::group(const QString& path, qint64& total)
{
    QMap<QString, qint64> groups;
    groupInto(path, groups, QFileInfo(path).fileName(), total);
    return groups;
}
