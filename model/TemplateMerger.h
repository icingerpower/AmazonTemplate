#ifndef TEMPLATEMERGER_H
#define TEMPLATEMERGER_H

#include <QString>
#include <QStringList>

#include <xlsxdocument.h>

class TemplateMerger
{
public:
    TemplateMerger(
        const QStringList &filePathsFrom,
        const QString &filePathTo);
    void exportTo(const QString &filePath);

private:
    void selectTemplateSheet(QXlsx::Document &doc);
    QStringList m_filePathsFrom;
    QString m_filePathTo;
};

#endif // TEMPLATEMERGER_H
