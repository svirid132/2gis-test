#include "FilterWordModel.h"
#include "RegExpIns.h"

#include <QFile>
#include <QTextStream>
#include <QTextCodec>

FilterWordModel::FilterWordModel(QObject *parent)
    : QStringListModel{parent}
{
    m_filename = "filter_word.txt";
    QStringList list;

    QFile file(m_filename);
    if (!file.open(QIODevice::ReadOnly)) {

    }
    QTextStream in(&file);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    in.setCodec(codec);
    while(!in.atEnd()) {
        QString filterWord;
        in >> filterWord;
        filterWord = filterWord.toLower();
        if (filterWord.contains(QRegExp(RegExpIns::m_filter))) {
            list.append(filterWord);
        }
    }
    file.close();

    this->setStringList(list);
}

void FilterWordModel::appendNewWord() {
    QString newWordTamplate = "новое-слово_%1";
    QString newWord;
    int ind = 1;
    do {
        newWord = newWordTamplate.arg(ind);
        ++ind;
    } while(stringList().contains(newWord));
    this->insertRow(this->rowCount());
    QModelIndex modelInd = index(this->rowCount() - 1);
    this->setData(modelInd, newWord);
}

void FilterWordModel::append(const QString &word)
{
    appendNewWord();
    edit(this->rowCount() - 1, word);
}

bool FilterWordModel::edit(int row, const QString &word)
{
    if (word.isEmpty() || stringList().contains(word)) {
        return false;
    }
    QModelIndex ind = index(row);
    this->setData(ind, word.toLower());
    return true;
}

void FilterWordModel::remove(int row)
{
    this->removeRow(row);
}

void FilterWordModel::remove(const QString& word) {
    QStringList list = this->stringList();
    for (int i = list.length() - 1; i > -1; --i) {
        if (list[i] == word) {
            this->remove(i);
            return;
        }
    }
}

void FilterWordModel::save()
{
    QStringList list = stringList();

    QFile file(m_filename);
    if (!file.open(QIODevice::WriteOnly)) {

    }
    QTextStream in(&file);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    in.setCodec(codec);
    std::for_each(list.begin(), list.end(), [&in](const QString& word) {
        in << word << "\r\n";
    });
    file.close();
}
