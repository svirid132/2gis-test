#include "rankwordfile.h"

#include "rankwordmodel.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QThread>

RankWordFile::RankWordFile(QObject *parent)
    : QObject{parent}
{
    qRegisterMetaType<QList<RankWord>>("QList<RankWord>");
}

void RankWordFile::read(const QString& fullpath)
{
    QFile file(fullpath);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "error";
        emit error();
        return;
    }

    QTextStream in(&file);

    QList<RankWord> rankWordList;
    rankWordList.reserve(16);
    for (int i = 0; i < 15; ++i) {
        rankWordList.append(RankWord());
    }
    QMap<QString, int> word_count;
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(" ");
        for (int i = 0; i < fields.length(); ++i) {
            fields[i] = fields[i].toLower().remove(QRegExp("[.,:!»()…?\"\[\\]]"));
        }
        QThread::sleep(1);
        fields = fields.filter(QRegExp("^(?!бы|за|не|то|из|под|на|до|во|по|что|он|как|это|мы|все|но|его|так|они|ты|же|меня|мне|когда|ему|от|если|их|еще|ее|ни|уже|вы|там|тут|тебя|тебе|себя)([a-zA-Z]+|[а-яА-Я-]+).$"));
        if (!fields.isEmpty()) {
            QSet<QString> wordSet;
            for (int i = 0; i < fields.length(); ++i) {
                word_count[fields[i]] += 1;
                wordSet.insert(fields[i]);
            }
            QSetIterator<QString> wordSetI(wordSet);
            while (wordSetI.hasNext()) {
                QString word = wordSetI.next();
                int count = word_count[word];
                if (count > rankWordList.at(14).count) {
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
                        rankWordList.removeAt(15);
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
