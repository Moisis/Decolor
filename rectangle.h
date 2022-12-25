#pragma once

#include <QRect>
#include <QPainter>
#include "Shape.h"



class Rectangle : public Shape
{
public:
    Rectangle(const QPoint &start, const QPoint &end, const int &penWidth, const QColor color) : Shape() {
        m_start = start;
        m_end = end;
        m_pen_width = penWidth;
        m_color = color;
    }
    void draw(QPainter &painter) const override;
    void erase(QPainter &painter) const override;
    void drag(const QPoint& offset) override;
    QRect boundingRect() const override;

};
