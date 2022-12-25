#include "cursor.h"

Cursor::Cursor()
    : mRect(0, 0, 0, 0)
{


}

void Cursor::setRect(const QRect &rect)
{
    mRect = rect;
}


void Cursor::selectShapes(const QList<Shape*> &shapes)
{
    for (Shape* shape : shapes)
    {
        if (mRect.intersects(shape->boundingRect()))
        {
            shape->select();
        }
    }
}

void Cursor::deselectShapes(const QList<Shape*> &shapes)
{
    for (Shape* shape : shapes)
    {
        if (mRect.intersects(shape->boundingRect()))
        {
            shape->deselect();
        }
    }
}
