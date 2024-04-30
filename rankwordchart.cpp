#include "RankWordChart.h"

#include "RankWordModel.h"
#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QPainter>
#include "RankWordModel.h"
#include <QMouseEvent>

RankWordChart::RankWordChart()
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);

    setImplicitWidth(1100);
    setImplicitHeight(500);

    RankWordModel& model = RankWordModel::getInstance();
    connect(&model, &RankWordModel::modelReset, this, [this]() { clearSelected(), this->update(); });
}

void RankWordChart::paint(QPainter *painter)
{
    if(styles == nullptr) {
        return;
    }

    painter->setRenderHint(QPainter::Antialiasing);

    // инцилизация даных модели
    RankWordModel& model = RankWordModel::getInstance();
    auto rankWords = model.rankWords();

    if (rankWords.count() == 0) {
        return;
    }

    // инцилизация слов
    QFont wordFont;
    wordFont.setPointSize(9);
    QFontMetrics wordFontMetrics(wordFont);

    // линия
    int bottomMarginLine = wordFontMetrics.height() * 3;
    QPointF lineP1 = QPointF(0, this->height() - bottomMarginLine);
    QPointF lineP2 = QPointF(this->width(), this->height() - bottomMarginLine);
    painter->drawLine(lineP1, lineP2);

    // инцилизация количество слов
    QFont numFont;
    numFont.setPointSize(12);
    QFontMetrics numFontMetrics(numFont);

    // колонки
    int topRank = model.maxRank() + (10 - model.maxRank() % 10);
    int columnCount = rankWords.count();
    int columnLeftRightPadding = 10;
    int columnSpacing = 15;
    int columnWidth = (this->width() - columnSpacing * (columnCount - 1) - columnLeftRightPadding * 2) / columnCount;
    m_columns.clear();
    painter->save();
    QColor columnColor = styles->property("chart_column_color").value<QColor>();
    painter->setBrush(columnColor);
    QPen columnPen("transparent");
    painter->setPen(columnPen);
    for (int i = 0; i < columnCount; ++i) {
        qreal columnHeight = numFontMetrics.height() + lineP1.y() - lineP1.y() * rankWords.at(i).count / topRank;
        float startY = std::min(columnHeight, lineP1.y());
        float startX = columnLeftRightPadding + columnWidth * i + columnSpacing * i;
        QPointF startP = QPointF(startX, startY);
        float endX = startP.x() + columnWidth;
        QPointF endP = QPointF(endX, lineP1.y());
        QRectF column = QRectF(startP, endP);
        m_columns.append(column);
    }
    for (int i = 0; i < columnCount; ++i) {
        if (!m_selected.contains(i) && hoveredIndex != i) {
            painter->drawRect(m_columns[i]);
        }
    }
    painter->restore();


    for (int i = 0; i < columnCount; ++i) {
        if (m_selected.contains(i) && hoveredIndex != i) {
            painter->save();
            columnColor = styles->property("selected_chart_column_color").value<QColor>();
            painter->setBrush(columnColor);
            columnPen = QPen("transparent");
            painter->setPen(columnPen);
            painter->drawRect(m_columns[i]);
            painter->restore();
        }
    }

    if (m_columns.size() > hoveredIndex && hoveredIndex != -1) {
        painter->save();
        columnColor = styles->property("hovered_chart_column_color").value<QColor>();
        painter->setBrush(columnColor);
        columnPen = QPen("transparent");
        painter->setPen(columnPen);
        painter->drawRect(m_columns[hoveredIndex]);
        painter->restore();
    }

    // количство слов
    painter->save();
    painter->setFont(numFont);
    for (int i = 0; i < columnCount; ++i) {
        qreal startPos = lineP1.y() - lineP1.y() * rankWords.at(i).count / topRank;
        qreal limitPos = lineP1.y() - numFontMetrics.height();
        float startY = std::min(startPos, limitPos);
        float startX = columnLeftRightPadding + columnWidth * i + columnSpacing * i;
        QPointF startP = QPointF(startX, startY);
        float endX = startP.x() + columnWidth;
        float endY = startY + numFontMetrics.height();
        QPointF endP = QPointF(endX, endY);
        QRectF rect = QRectF(startP, endP);
        painter->drawText(rect, Qt::AlignHCenter, QString::number(rankWords.at(i).count));
    }
    painter->restore();

    // слова
    painter->setFont(wordFont);
    for (int i = 0; i < columnCount; ++i) {
        QTextOption textOption;
        textOption.setWrapMode(QTextOption::WrapAnywhere);
        textOption.setAlignment(Qt::AlignHCenter);
        QPointF startP = QPointF(columnLeftRightPadding + columnWidth * i + columnSpacing * i, lineP1.y());
        QPointF endP = QPointF(startP.x() + columnWidth, this->height());
        painter->drawText(QRectF(startP, endP), rankWords.at(i).word, textOption);
    }
}

void RankWordChart::componentComplete()
{
    QQuickWindow *rootObject = window();
    styles = rootObject->property("styles").value<QObject*>();
    update();
    QQuickPaintedItem::componentComplete();
}

void RankWordChart::clearSelected()
{
    m_selected.clear();
    update();
}

void RankWordChart::switchSelectedColumn(int index, bool isSelected)
{
    if (isSelected) {
        m_selected.append(index);
    } else {
        m_selected.removeAll(index);
    }
    update();
}

void RankWordChart::hoverMoveEvent(QHoverEvent *event)
{
    for(int i = 0; i < m_columns.length(); ++i) {
        if (m_columns[i].contains(event->posF())) {
            hoveredIndex = i;
            update();
            return;
        }
    }
    hoveredIndex = -1;
    update();
}

void RankWordChart::mousePressEvent(QMouseEvent *event)
{
    // инцилизация даных модели
    RankWordModel& model = RankWordModel::getInstance();
    auto rankWords = model.rankWords();

    for(int i = 0; i < m_columns.length(); ++i) {
        if (m_columns[i].contains(event->localPos())) {
            bool isSelected = m_selected.contains(i);
            emit clickByColumn(i, rankWords.at(i).word, isSelected);
            update();
        }
    }
}

void RankWordChart::hoverLeaveEvent(QHoverEvent *event)
{
    Q_UNUSED(event)

    hoveredIndex = -1;
    update();
}
