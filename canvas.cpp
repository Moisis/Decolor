#include "canvas.h"
#include <QMouseEvent>
#include <QPainter>
#include <iostream>
#include <QDebug>
#include <queue>
#include<QRect>
#include<QStack>

#include "rectangle.h"
#include "ellipse.h"
#include "line.h"
#include "cursor.h"


Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
}


/*-----------------------------------------------------------------Setters------------------------------------------------------------------------*/

void Canvas::setmode(bool newmode){
  this->dark = newmode ;
    clearImage();

}
void Canvas::setpicker(bool newmode){
  this->picker = newmode ;

}

void Canvas::setTool(std::string dShape) {
    if (dShape == "line") {
       tool = Tools::LINE;
    } else if (dShape == "rect") {
       tool = Tools::RECT;
    } else if (dShape == "circle") {
       tool = Tools::CIRCLE;
    } else if (dShape == "free") {
       tool = Tools::BRUSH;
    } else if(dShape == "fill"){
       tool = Tools::FILL;
    } else if (dShape == "eraser"){
       tool = Tools::ERASER;
    } else if (dShape == "cursor"){

       tool = Tools::CURSOR;
    }else if (dShape == "colorpick" ){
        tool = Tools::ColorPick ;
    }
}

void Canvas::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void Canvas::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void Canvas::clearImage()
{
    if (dark) {
        backgroundColor = QColor(0, 0, 0);
    } else {
        backgroundColor = QColor(255, 255, 255);
    }
    image.fill(backgroundColor);
    modified = false;
    update();
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
    if (!begin.isNull() && !dest.isNull()) {
        painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                Qt::RoundJoin));
        painter.setRenderHint(QPainter::Antialiasing);
//        if (previewShape != nullptr) {
//            previewShape->draw(painter);
//        }
    }
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}


/*-----------------------------------------------------------------Mouse Event handling---------------------------------------------------------*/
void Canvas::mousePressEvent(QMouseEvent *event)
{
    begin = event->pos();
    dest = begin;
    lastPoint = event->pos();
    scribbling = true;
    dragging = true;
    if (event->button() == Qt::LeftButton) {
        switch(tool) {
            case FILL: {
                isLastCommandShapeUndo.push(false);
                deselect();
                undoStack.push(image);
                lastPoint = event->pos();
                points.push(QPoint(event->pos().x(), event->pos().y()));
                eventColor = image.pixelColor(event->pos().x(), event->pos().y());
                floodFill();
                scribbling = true;
                break;
            }
            case BRUSH: {
                isLastCommandShapeUndo.push(false);
                deselect();
                undoStack.push(image);
                begin = event->pos();
                dest = begin;
                lastPoint = event->pos();
                scribbling = true;
                break;
            }
            case ERASER: {
                isLastCommandShapeUndo.push(false);
                deselect();
                undoStack.push(image);
                begin = event->pos();
                dest = begin;
                lastPoint = event->pos();
                scribbling = true;
                break;
            }
            case LINE: {
                deselect();
                previewShape = new Line(begin, dest, myPenWidth, myPenColor);
                break;
            }
            case RECT: {
                deselect();
                previewShape =  new Rectangle(begin, dest, myPenWidth, myPenColor);
                break;
            }
            case CIRCLE: {
                deselect();
                previewShape =  new Ellipse(begin, dest, myPenWidth, myPenColor);
                break;
        }

            default: {
                break;
            }
        }
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (tool == Tools::BRUSH) {
        if ((event->buttons() & Qt::LeftButton) && scribbling)
            drawLineTo(event->pos(), myPenColor);
    } else if (tool == Tools::ERASER) {
        if ((event->buttons() & Qt::LeftButton) && scribbling)
            erase(event->pos());
    } else if (tool == Tools::CURSOR) {
        cursor.setRect(QRect(begin, dest));
        if (event->buttons() & Qt::LeftButton) {
            if (dragging) {
                // Update the position of the selected shapes
                QPainter painter(&image);
                QPoint offset = event->pos() - dest;
                clearImage();
                for (Shape* shape : shapes) {
                    if (shape->selected()  ) {
                        shape->drag(offset);


                    }else{
                            painter.setPen(QPen(shape->color(), shape->penWidth(), Qt::SolidLine, Qt::RoundCap,
                                Qt::RoundJoin));
                        shape->draw(painter);
                    }
                }
                update();
            }
            else {
                cursor.selectShapes(shapes);
                update();
            }
        }
        else {
            cursor.deselectShapes(shapes);
            update();
        }
        dest = event->pos();
    } else if (tool == Tools::ColorPick)
    {

    }else {
        if ((event->buttons() & Qt::LeftButton) && scribbling) {
            dest = event->pos();
            previewShape->setEndPoint(dest);
            update();

    }
    }
}


void Canvas::mouseReleaseEvent(QMouseEvent *event)
{
    QPainter painter(this);
    dragging = false;
    if (tool == Tools::BRUSH) {
        if (event->button() == Qt::LeftButton && scribbling) {
            drawLineTo(event->pos(), myPenColor);
            scribbling = false;
        }
    }
    else if (tool == Tools::LINE) {
        if ((event->button() & Qt::LeftButton) && scribbling) {
            addToShapes(new Line(begin, dest, myPenWidth, myPenColor));
            update();
            scribbling = false;
        }
    }
    else if (tool == Tools::RECT) {
        if ((event->button() & Qt::LeftButton) && scribbling) {
            addToShapes(new Rectangle(begin, dest, myPenWidth, myPenColor));
            update();
            scribbling = false;

        }
    } else if (tool == Tools::CIRCLE) {
        if ((event->button() & Qt::LeftButton) && scribbling) {
            addToShapes(new Ellipse(begin, dest, myPenWidth, myPenColor));
            update();
            scribbling = false;
        }
    } else if(tool == Tools::ERASER) {
            erase(event->pos());
            scribbling = false;
    } else if (tool == Tools::CURSOR) {
        select();
        update();
   scribbling = false;


    }else if (tool == Tools::ColorPick) {

        this->setPenColor(image.pixelColor(event->pos()));

    }

}


/*---------------------------------------------------------------------Tools functions------------------------------------------------------------*/

void Canvas::drawLineTo(const QPoint &endPoint, QColor color)
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(color, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;
    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));

    lastPoint = endPoint;

}

/* fill tool */
void Canvas::floodFill() {
    std::array<int, 4> dx = {0, 1, 0, -1};
    std::array<int, 4> dy = {1, 0, -1, 0};

    if (!(myPenColor == eventColor)) {
        while(!points.empty()) {
            int x = points.front().x();
            int y = points.front().y();
            points.pop();
            for (int i = 0; i < 4; i++) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                QColor newColor = image.pixelColor(nx, ny);
                if (isValidImageIndex(nx, ny) && newColor == eventColor) {
                    points.push(QPoint(nx, ny));
                    image.setPixelColor(nx, ny, myPenColor);
                    update();
                }
            }
        }
    }
}

/* eraser tool */
void Canvas::erase(const QPoint &endPoint) {
    drawLineTo(endPoint, backgroundColor);
}

/* select tool */
void Canvas::select() {
    cursor.setRect(QRect(begin, dest));
    cursor.selectShapes(shapes);
    QPainter painter(&image);
    for (Shape* shape : shapes) {
        painter.setPen(QPen(Qt::blue, shape->penWidth(), Qt::SolidLine, Qt::RoundCap,
            Qt::RoundJoin));
        if (shape->selected()) {
            shape->draw(painter);
        }
    }
}

/* deselect */
void Canvas::deselect() {
    for (Shape* shape : shapes) {
        QPainter painter(&image);
        painter.setPen(QPen(shape->color(), shape->penWidth(), Qt::SolidLine, Qt::RoundCap,
            Qt::RoundJoin));
        if (shape->selected()) {
            shape->deselect();
            shape->draw(painter);
        }
    }

}


/*--------------------------------------------------------------------Misc------------------------------------------------------------------------*/
void Canvas::undo() {
    if (isLastCommandShapeUndo.top()) {
        if(!shapes.empty()) {
            Shape* shape = shapes.back();
            QPainter painter(&image);
            shape->erase(painter);
            shapesRedo.push_back(shape);
            shapes.pop_back();
            update();
        }
    } else {
        redoStack.push(image);
        if (!undoStack.empty()) {
            image = undoStack.top();
            undoStack.pop();
            update();
        } else{
            modified = false ;
        }
    }
    if (!isLastCommandShapeUndo.empty()) {
        isLastCommandShapeRedo.push(isLastCommandShapeUndo.top());
        isLastCommandShapeUndo.pop();
    }
}

void Canvas::redo() {
    if (isLastCommandShapeRedo.top()) {
        if(!shapesRedo.empty()) {
            Shape* shape = shapesRedo.back();
            addToShapes(shape);
            shapesRedo.pop_back();
            update();
        }
    } else {
        undoStack.push(image);
        if (!redoStack.empty()) {
            image = redoStack.top();
            update();
            redoStack.pop();
        } else {
            modified = false ;
        }
    }
    if (!isLastCommandShapeRedo.empty()) {
        isLastCommandShapeUndo.push(isLastCommandShapeRedo.top());
        isLastCommandShapeRedo.pop();
    }
}

bool Canvas::openImage(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool Canvas::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    }
    return false;
}

void Canvas::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(backgroundColor);
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}


/*--------------------------------------------------------------------------Helper functions------------------------------------------------------*/
bool Canvas::isValidImageIndex(int x, int y) {
    return x >= 0 && y >= 0 && x < image.width() && y < image.height();
}

void Canvas::addToShapes(Shape* shape) {
    isLastCommandShapeUndo.push(true);
    shapes.append(shape);
    undoStack.push(image);
    QPen pen(shape->color(), shape->penWidth(), Qt::SolidLine, Qt::RoundCap,
            Qt::RoundJoin);
    QPainter imagePainter(&image);
    imagePainter.setPen(pen);
    shape->draw(imagePainter);
}
