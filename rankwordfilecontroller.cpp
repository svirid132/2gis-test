#include "rankwordfilecontroller.h"

#include "rankwordfile.h"

#include <QDebug>
#include <QThread>

RankWordFileController::RankWordFileController(QObject *parent)
    : QObject{parent}
{}

QString RankWordFileController::fullpath() const
{
    return m_fullpath;
}

void RankWordFileController::setFullpath(const QString &newFullpath)
{
    if (m_fullpath == newFullpath)
        return;
    m_fullpath = newFullpath;
    emit fullpathChanged();
}

void RankWordFileController::read()
{
    RankWordFile* file = new RankWordFile();
    QThread* thread = new QThread();
    file->moveToThread(thread);
    connect(file, &RankWordFile::restResult, this, [this](float progress, QList<RankWord> rankWordList) {
        // qDebug() << progress;
        RankWordModel& model = RankWordModel::getInstance();
        model.setRankWords(rankWordList);
        setProgress(progress);
    });
    connect(file, &RankWordFile::finished, thread, &QThread::quit);
    connect(file, &RankWordFile::finished, file, []() {
        qDebug() << "this is qqq";
    });
    connect(thread, &QThread::finished, thread, []() {
        qDebug() << "this is finished";
    });
    connect(thread, &QThread::started, file, std::bind(&RankWordFile::read, file, "E:/временное/Твен Марк. Приключения Тома Сойера и Гекльберри Финна. Большой сборник - royallib.com.txt"));
    thread->start();
}

float RankWordFileController::progress() const
{
    return m_progress;
}

void RankWordFileController::setProgress(float newProgress)
{
    if (m_progress == newProgress)
        return;
    m_progress = newProgress;
    emit progressChanged();
}
