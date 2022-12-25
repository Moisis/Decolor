#pragma once
#include <QPoint>
#include <memory>
#include <QPainter>

class Shape
{
public:
    Shape() = default;
    virtual ~Shape() = default;

    virtual void draw(QPainter &painter) const = 0;
    virtual void erase(QPainter &painter) const = 0;

    // Getters
    int PenWidth() const { return pen_width; }
    QPoint StartPoint() const { return m_start; }
    QPoint EndPoint() const { return m_end; }

    // Setters
    void setPenWidth(int width) { pen_width = width; }
    void setStartPoint(const QPoint& start) { m_start = start; }
    void setEndPoint(const QPoint& end) { m_end = end; }



protected:
    int pen_width;
    QPoint m_start;
    QPoint m_end;
};

using ShapePtr = std::unique_ptr<Shape>;
