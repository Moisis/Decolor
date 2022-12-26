#ifndef CANVAS_H
#define CANVAS_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QRect>
#include <QStack>
#include <QUndoCommand>
#include <memory>

#include "Stack.h"
#include "Queue.h"
#include "Shape.h"
#include "cursor.h"

class Canvas : public QWidget
{
    Q_OBJECT

public:
    Canvas(QWidget *parent = nullptr);
    enum Tools {
        CURSOR,
        BRUSH,
        ERASER,
        LINE,
        RECT,
        CIRCLE,
        FILL,
        ColorPick,
    };
    bool picker= false ;
    bool dark = true;
    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);

    // getters
    int penWidth() const { return myPenWidth; }
    bool isModified() const { return modified; }

    // setters
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    void setTool(std::string sh);
    void setLastRect(const QPoint &p1, const QPoint &p2);
    void setmode(bool newmode);
    void setpicker(bool newmode);

    void undo();
    void redo();
    void addToShapes(Shape* shape);

    QStack <QWidget> st1;
    QColor penColor() const { return myPenColor; }
    Stack<QImage> undoStack;
    Stack<QImage> redoStack;

public slots:
    void clearImage();
    //
     void deselect();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

/*Tools functions*/
private:
    void drawLineTo(const QPoint &endPoint, QColor color);
    void resizeImage(QImage *image, const QSize &newSize);
    void erase(const QPoint &endPoint);
    void select();

    void floodFill();


/* Helper functions */
private:
    bool isValidImageIndex(int x, int y);


/* private members*/
private:
    Queue<QPoint> points;
    Tools tool = Tools::RECT;
    bool dragging = false;
    bool scribbling = false;
    bool modified = false;
    int myPenWidth = 3;
    QList<Shape*> shapes;
    QList<Shape*> shapesRedo;
    QColor eventColor;
    QColor backgroundColor = QColor(255, 255, 255);
    QColor myPenColor = QColor(223, 42, 42);
    QImage image;
    QPoint begin = QPoint();
    QPoint dest = QPoint();
    QPoint lastPoint;
    Cursor cursor;
    Stack<bool> isLastCommandShapeUndo;
    Stack<bool> isLastCommandShapeRedo;
    Shape* previewShape;
};


#endif


