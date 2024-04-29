#ifndef REG_EXP_INS_H
#define REG_EXP_INS_H

#include <QString>
#include <QObject>

class RegExpIns : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString filter READ filter NOTIFY filterChanged FINAL)

public:
    static QString m_filter;
    QString filter() const;

signals:
    void filterChanged();
};

#endif // REG_EXP_H
