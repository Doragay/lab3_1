#ifndef SIZEGROUPER_H
#define SIZEGROUPER_H

#include <QMap>

class SizeGrouper
{
public:
    virtual ~SizeGrouper();
    virtual QMap<QString, qint64> group(const QString& path, qint64& total);

protected:
    virtual void groupInto(const QString& path, QMap<QString, qint64>& groups, const QString& currentDir, qint64& total) = 0;
};

#endif // SIZEGROUPER_H
