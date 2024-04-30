#ifndef RANKWORDFILECONTROLLER_H
#define RANKWORDFILECONTROLLER_H

#include "RankWordFile.h"

#include <QMutex>
#include <QObject>
#include <QWaitCondition>

class RankWordFileController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fullpath READ fullpath WRITE setFullpath NOTIFY fullpathChanged FINAL)
    Q_PROPERTY(float progress READ progress WRITE setProgress NOTIFY progressChanged FINAL)
    Q_PROPERTY(bool reading READ reading NOTIFY readingChanged FINAL)
    Q_PROPERTY(bool started READ started NOTIFY startedChanged FINAL)
    Q_PROPERTY(NOTIFY error FINAL)
public:
    explicit RankWordFileController(QObject *parent = nullptr);

    QString fullpath() const;
    void setFullpath(const QString &newFullpath);
    Q_INVOKABLE void read();
    Q_INVOKABLE void resume();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void cancel();
    float progress() const;
    void setProgress(float newProgress);
    bool reading() const;

    bool started() const;

signals:
    void fullpathChanged();
    void progressChanged();
    void readingChanged();
    void error();

    void startedChanged();

private:
    QString m_fullpath;
    float m_progress = 0;
    bool m_reading = false;
    RankWordFile* m_file = nullptr;
    QThread* m_thread = nullptr;
    bool m_started = false;
};

#endif // RANKWORDFILECONTROLLER_H
