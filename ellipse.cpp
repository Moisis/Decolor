#include "ellipse.h"

void Ellipse::draw(QPainter &painter) const
{
    QRect rect(m_start, m_end);
    painter.drawEllipse(rect);
}

void Ellipse::erase(QPainter &painter) const {
    QRect rect(m_start, m_end);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.setPen(QPen(QColor(Qt::transparent), m_pen_width + 2));
    painter.drawEllipse(QRect(m_start, m_end));
}

QRect Ellipse::boundingRect() const {
    return QRect(m_start, m_end);
}

void Ellipse::drag(const QPoint &offset) {
    m_start += offset;
    m_end += offset;
}
