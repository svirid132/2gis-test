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
    Q_INVOKABLE void clearSelected();
    Q_INVOKABLE void switchSelectedColumn(int index, bool isSelected);

private:
    QObject* styles = nullptr;
    QVector<QRectF> m_columns;
    int hoveredIndex = -1;
    QList<int> m_selected = {};
    QStringList m_rankWords = {};

    // QQuickItem interface
protected:
    void hoverMoveEvent(QHoverEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void hoverLeaveEvent(QHoverEvent *event) override;

signals:
    void clickByColumn(int index, const QString& word, bool isSelected);
};

#endif // RANKWORDCHART_H
