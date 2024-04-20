#ifndef RANKWORDCHART_H
#define RANKWORDCHART_H

#include <QObject>
#include <QQuickPaintedItem>

class RankWordChart : public QQuickPaintedItem
{
    Q_OBJECT
public:
    RankWordChart();

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter) override;
    void componentComplete() override;

private:
    QObject* styles = nullptr;
};

#endif // RANKWORDCHART_H
