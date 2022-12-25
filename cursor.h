#ifndef CURSOR_H
#define CURSOR_H

#include <QRect>
#include <QList>
#include "Shape.h"


class Cursor
{
public:
    Cursor();
    void setRect(const QRect &rect);
    void selectShapes(const QList<Shape*> &shapes);
    void deselectShapes(const QList<Shape*> &shapes);

private:
    QRect mRect;
};


#endif // CURSOR_H
