#ifndef RANKWORDFILECONTROLLER_H
#define RANKWORDFILECONTROLLER_H

#include <QObject>

class RankWordFileController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fullpath READ fullpath WRITE setFullpath NOTIFY fullpathChanged FINAL)
    Q_PROPERTY(float progress READ progress WRITE setProgress NOTIFY progressChanged FINAL)
public:
    explicit RankWordFileController(QObject *parent = nullptr);

    QString fullpath() const;
    void setFullpath(const QString &newFullpath);

    Q_INVOKABLE void read();

    float progress() const;
    void setProgress(float newProgress);

signals:
    void fullpathChanged();
    void progressChanged();

private:
    QString m_fullpath;
    float m_progress = 0;
};

#endif // RANKWORDFILECONTROLLER_H
