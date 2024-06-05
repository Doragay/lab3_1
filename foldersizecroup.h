
#ifndef FOLDERSIZECROUP_H
#define FOLDERSIZECROUP_H
#include "SizeGrouper.h"

class FolderSizeGrouper: public SizeGrouper
{
protected:
    void groupInto(const QString& path, QMap<QString, qint64>& groups, const QString& currentDir, qint64& total) override;
};

#endif // FOLDERSIZECROUP_H
