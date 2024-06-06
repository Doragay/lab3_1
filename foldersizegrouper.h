#ifndef FOLDERSIZEGROUPER_H
#define FOLDERSIZEGROUPER_H

#include "sizegrouper.h"

class FolderSizeGrouper : public SizeGrouper
{
protected:
    void groupInto(const QString& path, QMap<QString, qint64>& groups, const QString& currentDir, qint64& total) ;
};

#endif // FOLDERSIZEGROUPER_H
