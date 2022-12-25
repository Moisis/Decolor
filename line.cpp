#include "line.h"

void Line::draw(QPainter &painter) const
{
    painter.drawLine(m_start, m_end);
}


void Line::erase(QPainter &painter) const
{
    painter.eraseRect(QRect(m_start, m_end));
}
