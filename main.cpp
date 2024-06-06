#include <QCoreApplication>
#include <QTextStream>
#include "foldersizegrouper.h"
#include "typesizegrouper.h"
#include <stdexcept>

class Context
{
public:
    void setSizeGrouper(SizeGrouper* grouper)
    {
        _grouper.reset(grouper);
    }

    QList<QString> makeGroup(const QString& path)
    {
        if (!_grouper) {
            return {};
        }

        QList<QString> strreprs;

        qint64 total = 0;
        auto group = _grouper->group(path, total);
        for (auto it = group.keyValueBegin(); it != group.keyValueEnd(); ++it) {
            QString ssize = QString::number(it->second / 1024);
            QString spercent = "*";
            if (total != 0) {
                auto percent = 100 * static_cast<double>(it->second) / total;
                spercent = percent > 0.01 ? QString::number(percent, 'f', 2) : "< 0.01";
            }
            QString str = it->first + " --- " + ssize + "kb --- " + spercent + '%';
            strreprs.append(str);
        }

        return strreprs;
    }

private:
    std::unique_ptr<SizeGrouper> _grouper;
};


SizeGrouper* makeGrouper(const QString& mode)
{
    if (mode == "folders") {
        return new FolderSizeGrouper;
    }
    if (mode == "types") {
        return new TypeSizeGrouper;
    }

    throw std::runtime_error("Unknown grouping mode '" + mode.toStdString() + "' when expected folders|types");

}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    Context ctx;

    while (true) {
        QTextStream in(stdin);
        QTextStream out(stdout);

        QString path = in.readLine();
        QString mode = in.readLine();

        QList<QString> strlist;
        try {
            ctx.setSizeGrouper(makeGrouper(mode));
            strlist = ctx.makeGroup(path);
        } catch (std::exception& ex) {
            out << "Error: " << ex.what() << '\n';
            continue;
        }

        for (const auto& x : strlist) {
            out << x << '\n';
        }
        out << "---------------------------------\n";
    }

    return app.exec();
}
