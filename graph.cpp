#include "graph.h"

#include <QPainter>
#include <QColor>
#include <QQmlEngine>

#include "filestatscontroller.h"

namespace Constants
{
constexpr static const uint32_t MAX_ITEM_COUNT      { 15 };
constexpr static const uint32_t TEXT_SPACING        { 20 };
constexpr static const float BAR_PADDING_MULTIPLIER { 0.9 };
} /// Constants

Graph::Graph()
{
    QObject::connect(FileStatsController::instance(), &FileStatsController::progressChanged, this, [this] () {
        update();
    });

    QObject::connect(FileStatsController::instance(), &FileStatsController::stateCleared, this, [this]() {
        update();
    });
}

Graph::~Graph()
{

}

void Graph::paint(QPainter* painter)
{
    const auto stats = FileStatsController::instance()->stats();

    if (stats.empty()) {
        return;
    }

    size_t itemCounter { 0 };
    const auto& maxValue = stats[0].second;
    const auto barWidth = static_cast<int>((width() / stats.size()) * Constants::BAR_PADDING_MULTIPLIER);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(Qt::blue);
    painter->setPen(Qt::blue);

    painter->fillRect(QRect(0, 0, width(), height()), Qt::white);

    QFont font = painter->font();
    font.setPixelSize(14);
    font.setWeight(QFont::Normal);
    QFontMetrics fmetrics(font);

    const auto textContainerHeight = fmetrics.height() * 2;

    for (const auto& [key, value] : stats) {
        //! Draw bars
        const auto scale = static_cast<float>(value) / maxValue;
        const auto barHeight = static_cast<int>(height() * scale);
        const auto x = static_cast<int>(qreal(itemCounter * (width() / stats.size())));
        const auto y = static_cast<int>(height() - barHeight - textContainerHeight - Constants::TEXT_SPACING);

        const auto r = QRect(x, y, barWidth, barHeight);
        painter->fillRect(r, Qt::blue);
        painter->drawRect(r);

        //! Draw lables
        painter->setFont(font);
        painter->setPen(Qt::black);

        const auto word = key;
        const auto occurances = QString::number(value);

        const auto textContainerWidth = std::max(word.size(), occurances.size()) * fmetrics.averageCharWidth();

        const auto point = QPoint(x + barWidth / 2 - textContainerWidth / 2, height() - textContainerHeight);
        const auto textContainer = QRect(point, QSize(textContainerWidth, textContainerHeight));

        painter->drawText(textContainer, Qt::AlignCenter, word + "\n" + occurances);

        ++itemCounter;
    }
}

void Graph::registerType()
{
    qmlRegisterType<Graph>("Graph", 1, 0, "Graph");
}
