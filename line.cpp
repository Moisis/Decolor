#include "line.h"

void Line::draw(QPainter &painter) const
{
    painter.drawLine(m_start, m_end);
}


void Line::erase(QPainter &painter) const
{
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.setPen(QPen(QColor(Qt::transparent), m_pen_width + 2));
    painter.drawLine(m_start, m_end);
}

QRect Line::boundingRect() const {
    return QRect(m_start, m_end);
}

void Line::drag(const QPoint &offset) {
    m_start += offset;
    m_end += offset;
}
