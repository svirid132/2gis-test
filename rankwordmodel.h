#ifndef RANKWORDMODEL_H
#define RANKWORDMODEL_H

#include <QStringListModel>

struct RankWord {
    QString word;
    int count = 0;
};

class RankWordModel : public QAbstractListModel
{
public:
    static RankWordModel& getInstance()
    {
        static RankWordModel instance;
        return instance;
    }

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    void setRankWords(const QList<RankWord>& list);
    QList<RankWord> rankWords();
    int maxRank();

private:
    RankWordModel();
    RankWordModel(RankWordModel const&) = delete;
    void operator=(RankWordModel const&) = delete;

    QList<RankWord> m_list;
};

Q_DECLARE_METATYPE(RankWord)

#endif // RANKWORDMODEL_H
