#include "rankwordfilecontroller.h"

#include "filterwordmodel.h"
#include "rankwordfile.h"

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
    if (m_file != nullptr) {
        return;
    }

    m_file = new RankWordFile();
    m_thread = new QThread();
    m_file->moveToThread(m_thread);
    connect(m_file, &RankWordFile::restResult, this, [this](float progress, QList<RankWord> rankWordList) {
        RankWordModel& model = RankWordModel::getInstance();
        model.setRankWords(rankWordList);
        setProgress(progress);
    });
    connect(m_file, &RankWordFile::finished, m_file, [this]() {
        m_file->deleteLater();
        m_file = nullptr;
        m_reading = false;
        emit readingChanged();
    });
    connect(m_file, &RankWordFile::error, m_file, [this]() {
        m_started = false;
        emit startedChanged();
        emit m_file->finished();
        emit this->error();
    });
    connect(m_file, &RankWordFile::finished, m_thread, &QThread::quit);
    connect(m_thread, &QThread::finished, m_thread, &QThread::deleteLater);
    FilterWordModel* fitlerWordModel = &FilterWordModel::getInstance();
    connect(m_thread, &QThread::started, m_file, std::bind(&RankWordFile::read, m_file, fullpath(), fitlerWordModel->stringList()));
    m_thread->start();

    m_reading = true;
    emit readingChanged();
    m_started = true;
    emit startedChanged();
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

void RankWordFileController::resume()
{
    if (m_file) {
        m_file->resume();
        m_reading = true;
        emit readingChanged();
    }
}

void RankWordFileController::pause()
{
    if (m_file) {
        m_file->pause();
        m_reading = false;
        emit readingChanged();
    }
}

void RankWordFileController::cancel()
{
    if (m_file) {
        m_thread->requestInterruption();
        m_file->resume(); // пробуждаем поток, чтобы завершить выполнение (при состоянии стоп)
    }

    m_started = false;
    emit startedChanged();

    RankWordModel& model = RankWordModel::getInstance();
    model.setRankWords({});
    setProgress(0);
}

bool RankWordFileController::reading() const
{
    return m_reading;
}

bool RankWordFileController::started() const
{
    return m_started;
}
