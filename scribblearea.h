#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QRect>
#include <QStack>
#include "Stack.h"
#include "Queue.h"
#include "Shape.h"
#include <memory>
//! [0]
class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    ScribbleArea(QWidget *parent = nullptr);
    enum Tools {
        Brush,
        Eraser,
        LINE,
        Rect,
        Circle,
        Fill,
        None,
    };
    Stack<QImage> undoStack;
    Stack<QImage> redoStack;
    bool picker= false ;
    bool dark = true;
    void setmode(bool newmode);
    void setpicker(bool newmode);
    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    bool isModified() const { return modified; }
    void setIsFloodFilling(bool val)  { isFloodFilling = val; }
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    void setTool(std::string sh);
    void setLastRect(const QPoint &p1, const QPoint &p2);
    void undo();
    void redo();
    int penWidth() const { return myPenWidth; }
    void addToShapes(Shape* shape);
    QStack <QWidget> st1;
    QColor penColor() const { return myPenColor; }

public slots:
    void clearImage();

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
    void floodFill();


/* Helper functions */
private:
    bool isValidImageIndex(int x, int y);


/* private members*/
private:
    Queue<QPoint> points;
    Tools tool = Tools::Rect;
    bool scribbling = false;
    bool modified = false;
    bool isFloodFilling = false;
    int myPenWidth = 3;
    QList<Shape*> shapes;
    QColor eventColor;
    QColor backgroundColor = QColor(255, 255, 255);
    QColor myPenColor = QColor(223, 42, 42);
    QImage image;
    QPoint begin = QPoint();
    QPoint dest = QPoint();
    QPoint lastPoint;
    Shape* previewShape;
};


#endif


