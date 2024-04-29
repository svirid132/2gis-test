#ifndef RANKWORDFILE_H
#define RANKWORDFILE_H

#include "rankwordmodel.h"

#include <QMutex>
#include <QObject>
#include <QWaitCondition>

class RankWordFile : public QObject
{
    Q_OBJECT
public:
    explicit RankWordFile(QObject *parent = nullptr);

    void read(const QString& fullpath, const QStringList& filter);
    void resume();
    void pause();
signals:
    void restResult(float progress, QList<RankWord> rankWordList);
    void finished();
    void error();

private:
    QMutex sync;
    QWaitCondition pauseCond;
    bool m_pause = false;
};


#endif // RANKWORDFILE_H
