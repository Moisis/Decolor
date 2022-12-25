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

QRect Ellipse::boundingRect() const {
    return QRect(m_start, m_end);
}

void Ellipse::drag(const QPoint &offset) {
    m_start += offset;
    m_end += offset;
}
