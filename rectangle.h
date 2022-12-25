#pragma once

#include <QRect>
#include <QPainter>
#include "Shape.h"



class Rectangle : public Shape
{
public:
    Rectangle(const QPoint &start, const QPoint &end, const int &penWidth) : Shape() {
        m_start = start;
        m_end = end;
        pen_width = penWidth;
    }
    void draw(QPainter &painter) const override;
    void erase(QPainter &painter) const override;

};
