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

void ScribbleArea::setDrawingShape(std::string dShape) {
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

/*************************
 *         Omar          *
 *************************/
void ScribbleArea::clearImage()
{
    if(this->dark){
    image.fill(qRgb(0, 0, 0));
    }
    else{
         image.fill(qRgb(255, 255, 255));
    }
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
    if (event->button() == Qt::LeftButton && tool == Tools::Fill) {
        lastPoint = event->pos();
        points.push(QPoint(event->pos().x(), event->pos().y()));
        eventColor = image.pixelColor(event->pos().x(), event->pos().y());
        floodFill();
        scribbling = true;
    } else if(event->button() == Qt::LeftButton) {
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
    if (tool == Tools::Pencil) {
        if (event->button() == Qt::LeftButton && scribbling) {
            drawLineTo(event->pos());
            scribbling = false;

        }
    }
    else if (tool == Tools::Line) {
        if ((event->button() & Qt::LeftButton) && scribbling) {
            drawLine(event->pos());
            scribbling = false;

        }
    }
    else if (tool == Tools::Rect) {
        if ((event->button() & Qt::LeftButton) && scribbling) {
            drawRect(event->pos());
            scribbling = false;

        }
    } else if (tool == Tools::Circle) {
        if ((event->button() & Qt::LeftButton) && scribbling) {
            drawEllipse(event->pos());
            scribbling = false;

        }
    }
}

/*---------------------------------------------------------------------Tools functions------------------------------------------------------------*/

/*Line tool*/
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

/*Rect tool*/
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

/*Circle tool*/
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
/* Pencil tool */
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

void ScribbleArea::floodFill() {
    std::array<int, 4> dx = {0, 1, 0, -1};
    std::array<int, 4> dy = {1, 0, -1, 0};

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
/*--------------------------------------------------------------------Misc------------------------------------------------------------------------*/

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


/*--------------------------------------------------------------------------Helper functions------------------------------------------------------*/
bool ScribbleArea::isValidImageIndex(int x, int y) {
    return x >= 0 && y >= 0 && x < image.width() && y < image.height();
}
