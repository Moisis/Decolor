/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "scribblearea.h"

#include <QMouseEvent>
#include <QPainter>
#include <iostream>

#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#include<QRect>
#include<QStack>

#endif
#endif



//! [0]
ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);


}
//! [0]

//! [1]
bool ScribbleArea::openImage(const QString &fileName)
//! [1] //! [2]
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
//! [2]

//! [3]
bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
//! [3] //! [4]
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    }
    return false;
}
//! [4]

//! [5]
void ScribbleArea::setPenColor(const QColor &newColor)
//! [5] //! [6]
{
    myPenColor = newColor;
}

void ScribbleArea::setPenWidth(int newWidth)

{
    myPenWidth = newWidth;
}

void ScribbleArea::clearImage()

{
    image.fill(qRgb(255, 255, 255));
    modified = false;
    update();
}
//! [12] //! [13]
void ScribbleArea::paintEvent(QPaintEvent *event)
//! [13] //! [14]
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
    if (!begin.isNull() && !dest.isNull()) {
        painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                Qt::RoundJoin));
        switch(myDrawingShape) {
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
//! [14]

//! [15]
void ScribbleArea::resizeEvent(QResizeEvent *event)
//! [15] //! [16]
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void ScribbleArea::setDrawingShape(std::string dShape) {
    if (dShape == "line") {
       myDrawingShape = DrawingShape::Line;
    } else if (dShape == "rect") {
       myDrawingShape = DrawingShape::Rect;
    } else if (dShape == "circle") {
       myDrawingShape = DrawingShape::Circle;
    } else if (dShape == "free") {
       myDrawingShape = DrawingShape::HandDrawing;
    }else{
       myDrawingShape = DrawingShape::None;
    }
}

//! [11]
void ScribbleArea::mousePressEvent(QMouseEvent *event)
//! [11] //! [12]
{
    if (event->button() == Qt::LeftButton) {

        begin = event->pos();
        dest = begin;
        lastPoint = event->pos();
        scribbling = true;
        }
    }


void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{

    if(myDrawingShape == DrawingShape::HandDrawing) {
        if ((event->buttons() & Qt::LeftButton) && scribbling)
            drawLineTo(event->pos());
    } else {
        if ((event->buttons() & Qt::LeftButton) && scribbling) {
            dest = event->pos();
            update();
        }
    }
}


void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (myDrawingShape == DrawingShape::HandDrawing) {
        if (event->button() == Qt::LeftButton && scribbling) {
            drawLineTo(event->pos());
            scribbling = false;

        }
    }
    else if (myDrawingShape == DrawingShape::Line) {
        if ((event->button() & Qt::LeftButton) && scribbling) {
            drawLine(event->pos());
            scribbling = false;

        }
    }
    else if (myDrawingShape == DrawingShape::Rect) {
        if ((event->button() & Qt::LeftButton) && scribbling) {
            drawRect(event->pos());
            scribbling = false;

        }
    } else if (myDrawingShape == DrawingShape::Circle) {
        if ((event->button() & Qt::LeftButton) && scribbling) {
            drawEllipse(event->pos());
            scribbling = false;

        }
    }
}

void ScribbleArea::drawLine(const QPoint &endPoint) {
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));


    painter.drawLine(begin, dest);
    modified = true;


    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                   .adjusted(-rad, -rad, +rad, +rad));


    lastPoint = endPoint;
}

/* helper function to get me the smaller between two points important when drwaing a trinagle */
QPoint ScribbleArea::smaller(const QPoint &lastPoint, const QPoint &endPoint) {
    if (lastPoint.x() < endPoint.x() && lastPoint.y() < endPoint.y()) {
        return lastPoint;
    } else {
        return endPoint;
    }
}

void ScribbleArea::drawRect(const QPoint &endPoint) {
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    QRect rect = QRect(begin, dest);
    painter.drawRect(rect);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                   .adjusted(-rad, -rad, +rad, +rad));

}

void ScribbleArea::drawEllipse(const QPoint &endPoint) {
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    QRect rect = QRect(begin, dest);
    painter.drawEllipse(rect);
    modified = true;

    int rad = (myPenWidth / 2) + 2;
   update(QRect(lastPoint, endPoint).normalized()
                                    .adjusted(-rad, -rad, +rad, +rad));


}




void ScribbleArea::drawLineTo(const QPoint &endPoint)

{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;
    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
                                     .adjusted(-rad, -rad, +rad, +rad));


    lastPoint = endPoint;

}
//! [18]

//! [19]
void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
//! [19] //! [20]
{
    if (image->size() == newSize)
        return;
    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
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

