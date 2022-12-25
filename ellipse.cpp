#include "ellipse.h"

void Ellipse::draw(QPainter &painter) const
{
    QRect rect(m_start, m_end);
    painter.drawEllipse(rect);
}

void Ellipse::erase(QPainter &painter) const {
    QRect rect(m_start, m_end);
    painter.eraseRect(rect);
}
