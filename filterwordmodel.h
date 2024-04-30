#ifndef FILTERWORDMODEL_H
#define FILTERWORDMODEL_H

#include <QObject>
#include <QStringListModel>

class FilterWordModel : public QStringListModel
{
    Q_OBJECT
public:
    static FilterWordModel& getInstance()
    {
        static FilterWordModel instance;
        return instance;
    }
    Q_INVOKABLE void save();
    Q_INVOKABLE void appendNewWord();
    Q_INVOKABLE void append(const QString& word);
    Q_INVOKABLE bool edit(int row, const QString& word);
    Q_INVOKABLE void remove(int row);
    Q_INVOKABLE void remove(const QString &word);

private:
    explicit FilterWordModel(QObject *parent = nullptr);
    QString m_filename;
    FilterWordModel(FilterWordModel const&) = delete;
    void operator=(FilterWordModel const&) = delete;

signals:
};

#endif // FILTERWORDMODEL_H
