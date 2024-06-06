
#ifndef TYPESIZEGROUPER_H
#define TYPESIZEGROUPER_H
#include "sizegrouper.h"

class TypeSizeGrouper: public SizeGrouper
{
protected:
    void groupInto(const QString& path, QMap<QString, qint64>& groups, const QString& currentDir, qint64& total);
};


#endif // TYPESIZEGROUPER_H
