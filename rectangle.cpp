#include "rectangle.h"

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
    painter.eraseRect(left, top, right - left, bottom - top);
}