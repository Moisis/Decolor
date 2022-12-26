#include "rectangle.h"
#include "canvas.h"

void Rectangle::draw(QPainter &painter) const
{
    int x1 = m_start.x();
    int y1 = m_start.y();
    int x2 = m_end.x();
    int y2 = m_end.y();
    int left = std::min(x1, x2);
    int top = std::min(y1, y2);
    int right = std::max(x1, x2);
    int bottom = std::max(y1, y2);
    painter.drawRect(left, top, right - left, bottom - top);
}

void Rectangle::erase(QPainter &painter) const {
    int x1 = m_start.x();
    int y1 = m_start.y();
    int x2 = m_end.x();
    int y2 = m_end.y();
    int left = std::min(x1, x2);
    int top = std::min(y1, y2);
    int right = std::max(x1, x2);
    int bottom = std::max(y1, y2);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.setPen(QPen(QColor(Qt::transparent), m_pen_width + 2));
    painter.drawRect(left, top, right - left, bottom - top);
}

QRect Rectangle::boundingRect() const {
    return QRect(m_start, m_end);
}

void Rectangle::drag(const QPoint &offset) {
    m_start += offset;
    m_end += offset;
}
