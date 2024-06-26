#include "RankWordFile.h"

#include "RankWordModel.h"
#include "RegExpIns.h"

#include <QFile>
#include <QSet>
#include <QTextStream>
#include <QThread>

RankWordFile::RankWordFile(QObject *parent)
    : QObject{parent}
{
    qRegisterMetaType<QList<RankWord>>("QList<RankWord>");
}

void RankWordFile::resume()
{
    sync.lock();
    m_pause = false;
    sync.unlock();
    pauseCond.wakeAll();
}

void RankWordFile::pause()
{
    sync.lock();
    m_pause = true;
    sync.unlock();
}

void RankWordFile::read(const QString& fullpath, const QStringList& filter)
{
    QFile file(fullpath);
    if(!file.open(QIODevice::ReadOnly)) {
        emit error();
        return;
    }

    QTextStream in(&file);

    int topWordCount = 15;
    QList<RankWord> rankWordList;
    rankWordList.reserve(topWordCount + 1);
    for (int i = 0; i < topWordCount; ++i) {
        rankWordList.append(RankWord());
    }
    QMap<QString, int> word_count;
    while(!in.atEnd()) {

        // пауза
        sync.lock();
        if(m_pause) {
            pauseCond.wait(&sync);
        }
        sync.unlock();

        // отмена
        if ( QThread::currentThread()->isInterruptionRequested() ) {
            break;
        }

        QString line = in.readLine();
        QStringList fields = line.split(" ");
        for (int i = 0; i < fields.length(); ++i) {
            fields[i] = fields[i].toLower().remove(QRegExp("[.,:!»()…?\"\[\\]]"));
        }
        fields = fields.filter(QRegExp(RegExpIns::m_filter));
        for (int i = fields.length() - 1; i > -1; --i) {
            if (filter.contains(fields[i])) {
                fields.removeAt(i);
            }
        }
        if (!fields.isEmpty()) {
            QSet<QString> wordSet;
            for (int i = 0; i < fields.length(); ++i) {
                word_count[fields[i]] += 1;
                wordSet.insert(fields[i]);
            }
            foreach (const QString& word, wordSet) {
                int count = word_count[word];
                int lastIndex = topWordCount - 1;
                if (count > rankWordList.at(lastIndex).count) {
                    bool isChangeCount = false;
                    for (int i = 0; i < rankWordList.count(); ++i) {
                        if (rankWordList[i].word == word) {
                            rankWordList[i].count = count;
                            isChangeCount = true;
                        }
                    }
                    if (!isChangeCount) {
                        rankWordList.append({word, count});
                    }
                    std::sort(rankWordList.begin(), rankWordList.end(), [](const RankWord &v1, const RankWord &v2) {
                        return v1.count > v2.count;
                    });
                    if (!isChangeCount) {
                        rankWordList.removeLast();
                    }
                }
                float currentProgress = (float)file.pos() / file.size();
                emit restResult(currentProgress, rankWordList);
            }
        }
    }
    emit finished();

    file.close();
}
