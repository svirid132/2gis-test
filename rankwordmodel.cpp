#include "rankwordmodel.h"

RankWordModel::RankWordModel() {
}

QVariant RankWordModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        const int row = index.row();
        return QVariant::fromValue( m_list.at(row) );
    }

    return QVariant();
}

int RankWordModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_list.count();
}

void RankWordModel::setRankWords(const QList<RankWord> &list)
{
    beginResetModel();
    m_list = list;
    endResetModel();
}

QList<RankWord> RankWordModel::rankWords()
{
    return m_list;
}

int RankWordModel::maxRank()
{
    int maxValue = 0;
    foreach (const RankWord& rankWord, m_list) {
        maxValue = rankWord.count > maxValue ? rankWord.count : maxValue;
    }

    return maxValue;
}
