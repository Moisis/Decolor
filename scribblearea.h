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

#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QRect>
#include <QStack>
#include <queue>
#include <stack>
//! [0]
class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    ScribbleArea(QWidget *parent = nullptr);
    enum Tools {
        Pencil,
        Eraser,
        Line,
        Rect,
        Circle,
        Fill,
        None,
    };
    bool dark = false;
    void setmode(bool newmode);

    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    bool isModified() const { return modified; }
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    void setTool(std::string sh);
    void setLastRect(const QPoint &p1, const QPoint &p2);
    void undo();
    void redo();
    int penWidth() const { return myPenWidth; }
    QStack <QWidget> st1;
    QColor penColor() const { return myPenColor; }

public slots:
    void clearImage();
    void print();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

/*Tools functions*/
private:
    void drawLineTo(const QPoint &endPoint, QColor color);
    void drawLine(const QPoint &endPoint);
    void drawRect(const QPoint &endPoint);
    void drawEllipse(const QPoint &endPoint);
    void resizeImage(QImage *image, const QSize &newSize);
    void erase(const QPoint &endPoint);
    void floodFill();


/* Helper functions */
private:
    bool isValidImageIndex(int x, int y);


/* private members*/
private:
    std::queue<QPoint> points;
    std::stack<QImage> undoStack;
    std::stack<QImage> redoStack;
    Tools tool = Tools::Rect;
    bool scribbling = false;
    bool modified = false;
    int myPenWidth = 3;
    QColor eventColor;
    QColor backgroundColor = QColor(0, 0, 0);
    QColor myPenColor = QColor(223, 42, 42);
    QImage image;
    QPoint begin = QPoint();
    QPoint dest = QPoint();
    QPoint lastPoint;
};


#endif


