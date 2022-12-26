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
    virtual QRect boundingRect() const = 0;
    virtual void drag(const QPoint& offset) = 0;
   //   virtual void resize(const QPoint& offset) = 0;

    // Getters
    int penWidth() const { return m_pen_width; }
    bool selected() const { return m_selected; }
    QColor color() const { return m_color; }
    QPoint StartPoint() const { return m_start; }
    QPoint EndPoint() const { return m_end; }

    // Setters
    void setPenWidth(int width) { m_pen_width = width; }
    void setColor(const QColor& c) { m_color = c ;}
    void select() { m_selected = true; }
    void deselect() { m_selected = false; }
    void setStartPoint(const QPoint& start) { m_start = start; }
    void setEndPoint(const QPoint& end) { m_end = end; }



protected:
    int m_pen_width;
    bool m_selected = false;
    QColor m_color;
    QColor m_highlight_color = Qt::blue;
    QPoint m_start;
    QPoint m_end;
};

