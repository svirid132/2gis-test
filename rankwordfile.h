#ifndef RANKWORDFILE_H
#define RANKWORDFILE_H

#include "rankwordmodel.h"

#include <QObject>

class RankWordFile : public QObject
{
    Q_OBJECT
public:
    explicit RankWordFile(QObject *parent = nullptr);

    Q_INVOKABLE void read(const QString& fullpath);
signals:
    void restResult(float progress, QList<RankWord> rankWordList);
    void finished();
    void error();

private:

};


#endif // RANKWORDFILE_H
