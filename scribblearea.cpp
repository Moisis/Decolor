#include "scribblearea.h"

#include <QMouseEvent>
#include <QPainter>
#include <iostream>
#include <QDebug>
#include <queue>

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#include<QRect>
#include<QStack>

#endif
#endif



ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
}


/*-----------------------------------------------------------------Setters------------------------------------------------------------------------*/

void ScribbleArea::setmode(bool newmode){
  this->dark = newmode ;
    clearImage();

}

void ScribbleArea::setTool(std::string dShape) {
    if (dShape == "line") {
       tool = Tools::Line;
    } else if (dShape == "rect") {
       tool = Tools::Rect;
    } else if (dShape == "circle") {
       tool = Tools::Circle;
    } else if (dShape == "free") {
       tool = Tools::Pencil;
    } else if(dShape == "fill"){
       tool = Tools::Fill;
    } else if (dShape == "eraser"){
       tool = Tools::Eraser;
    } else {
       tool = Tools::None;
    }
}

void ScribbleArea::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void ScribbleArea::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
}

void ScribbleArea::clearImage()
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

void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
    if (!begin.isNull() && !dest.isNull()) {
        painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                Qt::RoundJoin));
        painter.setRenderHint(QPainter::Antialiasing);
        switch(tool) {
            case Rect: {
                QRect rect = QRect(begin, dest);
                painter.drawRect(rect.normalized());
                break;
            }
            case Circle: {
                QRect rect = QRect(begin, dest);
                painter.drawEllipse(rect.normalized());
                break;
            }
            case Line: {
                painter.drawLine(begin, dest);
                break;
            }
            default:
                break;
        }
    }
}

void ScribbleArea::resizeEvent(QResizeEvent *event)
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
void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    begin = event->pos();
    dest = begin;
    lastPoint = event->pos();
    scribbling = true;
    if (event->button() == Qt::LeftButton && tool == Tools::Fill) {
        isFloodFilling = false;
        undoStack.push(image);
        lastPoint = event->pos();
        points.push(QPoint(event->pos().x(), event->pos().y()));
        eventColor = image.pixelColor(event->pos().x(), event->pos().y());
        floodFill();
        isFloodFilling = true;
        scribbling = true;
    } else if(event->button() == Qt::LeftButton && tool == Tools::Pencil) {
        undoStack.push(image);
        begin = event->pos();
        dest = begin;
        lastPoint = event->pos();
        scribbling = true;
    } else if (event->button() == Qt::LeftButton && tool == Tools::Eraser) {
        undoStack.push(image);
        begin = event->pos();
        dest = begin;
        lastPoint = event->pos();
        scribbling = true;
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if(tool == Tools::Pencil) {
        if ((event->buttons() & Qt::LeftButton) && scribbling)
            drawLineTo(event->pos(), myPenColor);
    } else if(tool == Tools::Eraser) {
        if ((event->buttons() & Qt::LeftButton) && scribbling)
            erase(event->pos());
    } else {
        if ((event->buttons() & Qt::LeftButton) && scribbling) {
            dest = event->pos();
            update();
        }
    }
}


void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (tool == Tools::Pencil) {
        if (event->button() == Qt::LeftButton && scribbling) {
            drawLineTo(event->pos(), myPenColor);
            scribbling = false;
        }
    }
    else if (tool == Tools::Line) {
        if ((event->button() & Qt::LeftButton) && scribbling) {
            undoStack.push(image);
            update();
            drawLine(event->pos());
            scribbling = false;

        }
    }
    else if (tool == Tools::Rect) {
        if ((event->button() & Qt::LeftButton) && scribbling) {
            undoStack.push(image);
            update();
            drawRect(event->pos());
            scribbling = false;

        }
    } else if (tool == Tools::Circle) {
        if ((event->button() & Qt::LeftButton) && scribbling) {
            undoStack.push(image);
            update();
            drawEllipse(event->pos());
            scribbling = false;
        }
    } else if(tool == Tools::Eraser) {
            erase(event->pos());
            scribbling = false;
    }
}

/*---------------------------------------------------------------------Tools functions------------------------------------------------------------*/

/*Line tool*/
void ScribbleArea::drawLine(const QPoint &endPoint) {
    QPainter painter(&image);
    //painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));


    painter.drawLine(begin, dest);
    modified = true;


    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                   .adjusted(-rad, -rad, +rad, +rad));


    lastPoint = endPoint;
}

/*Rect tool*/
void ScribbleArea::drawRect(const QPoint &endPoint) {
    QPainter painter(&image);
    //painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    QRect rect = QRect(begin, dest);
    painter.drawRect(rect);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                   .adjusted(-rad, -rad, +rad, +rad));
}

/*Circle tool*/
void ScribbleArea::drawEllipse(const QPoint &endPoint) {
    QPainter painter(&image);
    //painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    QRect rect = QRect(begin, dest);
    painter.drawEllipse(rect);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
   update(QRect(lastPoint, endPoint).normalized()
                                    .adjusted(-rad, -rad, +rad, +rad));
}
/* Pencil tool */
void ScribbleArea::drawLineTo(const QPoint &endPoint, QColor color)
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
void ScribbleArea::floodFill() {
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
                if (newColor == eventColor) {
                    points.push(QPoint(nx, ny));
                    image.setPixelColor(nx, ny, myPenColor);
                    update();
                }
            }
        }
    }
}

/* eraser tool */
void ScribbleArea::erase(const QPoint &endPoint) {
    drawLineTo(endPoint, backgroundColor);
}
/*--------------------------------------------------------------------Misc------------------------------------------------------------------------*/
void ScribbleArea::undo() {
    redoStack.push(image);
    floodFill();
    if (!undoStack.empty()) {
        image = undoStack.top();
        update();
        undoStack.pop();
    }
}

void ScribbleArea::redo() {
    undoStack.push(image);
    if (!redoStack.empty()) {
        image = redoStack.top();
        update();
        redoStack.pop();
    }
}
bool ScribbleArea::openImage(const QString &fileName)
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

bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    }
    return false;
}

void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(backgroundColor);
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void ScribbleArea::print()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
#endif // QT_CONFIG(printdialog)
}


/*--------------------------------------------------------------------------Helper functions------------------------------------------------------*/
bool ScribbleArea::isValidImageIndex(int x, int y) {
    return x >= 0 && y >= 0 && x < image.width() && y < image.height();
}
