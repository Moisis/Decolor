#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scribblearea.h"

#include <QApplication>
#include <QColorDialog>
#include <QFileDialog>
#include <QImageWriter>
#include <QInputDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QCloseEvent>
#include <QWidget>
#include <QGraphicsScene>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) ,ui(new Ui::MainWindow)
  , scribbleArea(new ScribbleArea(this))
{
    ui->setupUi(this);
    this->setCentralWidget(scribbleArea);
    scribbleArea->setDrawingShape("None");
    scribbleArea->clearImage();
}


MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
        event->accept();
    else
        event->ignore();
}
bool MainWindow::maybeSave()

{
    if (scribbleArea->isModified()) {
       QMessageBox::StandardButton ret;
       ret = QMessageBox::warning(this, tr("Decolor"),
                          tr("The image has been modified.\n"
                             "Do you want to save your changes?"),
                          QMessageBox::Save | QMessageBox::Discard
                          | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveFile("png");
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}
bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                               initialPath,
                               tr("%1 Files (*.%2);;All Files (*)")
                               .arg(QString::fromLatin1(fileFormat.toUpper()))
                               .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty())
        return false;
    return scribbleArea->saveImage(fileName, fileFormat.constData());
}

void MainWindow::on_actionCircle_triggered()
{
    scribbleArea->setDrawingShape("circle");
    ui->actionCursor->setChecked(false);
    ui->actionSquare->setChecked(false);
    ui->actionLine->setChecked(false);
    ui->actionFill->setChecked(false);
    ui->actionpencil->setChecked(false);
}


void MainWindow::on_actionCursor_triggered()
{
    ui->actionCircle->setChecked(false);
    ui->actionSquare->setChecked(false);
    ui->actionLine->setChecked(false);
    ui->actionFill->setChecked(false);
    ui->actionpencil->setChecked(false);
    scribbleArea->setDrawingShape("None");
}


void MainWindow::on_actionLine_triggered()
{
    ui->actionpencil->setChecked(false);
    ui->actionCursor->setChecked(false);
    ui->actionSquare->setChecked(false);
    ui->actionCircle->setChecked(false);
    ui->actionFill->setChecked(false);
    scribbleArea->setDrawingShape("line");

}


void MainWindow::on_actionFill_triggered()
{
    ui->actionCursor->setChecked(false);
    ui->actionSquare->setChecked(false);
    ui->actionCircle->setChecked(false);
    ui->actionLine->setChecked(false);
}


void MainWindow::on_actionSquare_triggered()
{
    ui->actionCursor->setChecked(false);
    ui->actionFill->setChecked(false);
    ui->actionCircle->setChecked(false);
    ui->actionLine->setChecked(false);
    ui->actionpencil->setChecked(false);
    scribbleArea->setDrawingShape("rect");

}


void MainWindow::on_actionOpen_triggered()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
            scribbleArea->openImage(fileName);
    }

}


void MainWindow::on_actionSave_as_triggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}


void MainWindow::on_actionPrint_triggered()
{
scribbleArea->print();
}


void MainWindow::on_actionExit_triggered()
{
    this->close();
}


void MainWindow::on_actionPen_Color_triggered()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());
    if (newColor.isValid())
        scribbleArea->setPenColor(newColor);
}


void MainWindow::on_actionPen_Width_triggered()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Decolor"),
                                        tr("Select pen width:"),
                                        scribbleArea->penWidth(),
                                        1, 50, 1, &ok);
    if (ok)
        scribbleArea->setPenWidth(newWidth);
}


void MainWindow::on_actionClear_Screen_triggered()
{
    scribbleArea->setDrawingShape("None");
    scribbleArea->clearImage();
}



void MainWindow::on_actionpencil_triggered()
{
    ui->actionCircle->setChecked(false);
    ui->actionSquare->setChecked(false);
    ui->actionLine->setChecked(false);
    ui->actionFill->setChecked(false);
    ui->actionCursor->setChecked(false);
    scribbleArea->setDrawingShape("free");
}


void MainWindow::on_actionUndo_triggered()
{




}


void MainWindow::on_actionredo_triggered()
{

}


void MainWindow::on_actionAbout_Project_triggered()
{
    QMessageBox::about(this, tr("About Decolor"),
            tr("<p>The <b>Decolor </b> is a data Structures Project "
               "An MS paint like application which allows you to draw Rectangles, "
               "Circle and various shapes, In addition to free hand drawing, packed with features like flood fill and and choosing colors and erasing. "
               ));
}

