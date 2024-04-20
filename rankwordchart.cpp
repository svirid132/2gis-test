#include "rankwordchart.h"

#include <QPainter>
#include <QDebug>
#include <QQmlEngine>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <RankWordModel.h>

RankWordChart::RankWordChart()
{
    setImplicitWidth(1100);
    setImplicitHeight(500);

    RankWordModel& model = RankWordModel::getInstance();
    connect(&model, &RankWordModel::modelReset, this, [this]() { this->update(); });
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
    QVector<QRectF> columns;
    painter->save();
    QColor columnColor = styles->property("chart_column_color").value<QColor>();
    painter->setBrush(columnColor);
    QPen columnPen("transparent");
    painter->setPen(columnPen);
    for (int i = 0; i < columnCount; ++i) {
        float currentY = numFontMetrics.height() + lineP1.y() - lineP1.y() * rankWords.at(i).count / topRank;
        QPointF currentP = QPointF(columnLeftRightPadding + columnWidth * i + columnSpacing * i, currentY);
        QPointF endP = QPointF(currentP.x() + columnWidth, lineP1.y());
        QRectF column = QRectF(currentP, endP);
        columns.append(column);
    }
    painter->drawRects(columns);
    painter->restore();

    // количство слов
    painter->save();
    painter->setFont(numFont);
    for (int i = 0; i < columnCount; ++i) {
        float currentY = lineP1.y() - lineP1.y() * rankWords.at(i).count / topRank;
        QPointF currentP = QPointF(columnLeftRightPadding + columnWidth * i + columnSpacing * i, currentY);
        QPointF endP = QPointF(currentP.x() + columnWidth, currentY + numFontMetrics.height());
        QRectF rect = QRectF(currentP, endP);
        painter->drawText(rect, Qt::AlignHCenter, QString::number(rankWords.at(i).count));
    }
    painter->restore();

    // слова
    painter->setFont(wordFont);
    for (int i = 0; i < columnCount; ++i) {
        QTextOption textOption;
        textOption.setWrapMode(QTextOption::WrapAnywhere);
        textOption.setAlignment(Qt::AlignHCenter);
        QPointF currentP = QPointF(columnLeftRightPadding + columnWidth * i + columnSpacing * i, lineP1.y());
        QPointF endP = QPointF(currentP.x() + columnWidth, this->height());
        painter->drawText(QRectF(currentP, endP), rankWords.at(i).word, textOption);
    }
}

void RankWordChart::componentComplete()
{
    QQuickWindow *rootObject = window();
    styles = rootObject->property("styles").value<QObject*>();
    update();
    QQuickPaintedItem::componentComplete();
}
