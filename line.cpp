#include "line.h"

void Line::draw(QPainter &painter) const
{
    painter.drawLine(m_start, m_end);
}


void Line::erase(QPainter &painter) const
{
    painter.eraseRect(QRect(m_start, m_end));
}

QRect Line::boundingRect() const {
    return QRect(m_start, m_end);
}

void Line::drag(const QPoint &offset) {
    m_start += offset;
    m_end += offset;
}
