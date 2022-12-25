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
#include <QGridLayout>
#include <QPushButton>
#include <QCursor>
#include <QSpinBox>
#include <QLabel>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) ,ui(new Ui::MainWindow)
  , scribbleArea(new ScribbleArea(this))
{
    ui->setupUi(this);
    this->setCentralWidget(scribbleArea);
    scribbleArea->setTool("None");
    ui->menubar1->setStyleSheet("background-color: #F0F1F3 ;");
    ui->toolBar2->setStyleSheet("background-color: #F0F1F3 ;");
    scribbleArea->setmode(false);
    QColorDialog *qtsd = new QColorDialog() ;
    qtsd->setWindowFlags(Qt::Widget);
    ui->toolBar2->addWidget(qtsd);
    ui->toolBar2->hide();
    qtsd->setOption(QColorDialog::NoButtons);
    qtsd->setOption(QColorDialog::DontUseNativeDialog);

    qtsd->connect(qtsd, &QColorDialog::currentColorChanged, [this, qtsd]()
                {
        scribbleArea->setPenColor(qtsd->currentColor());
    }
    );
    QLabel *penwidthlbl = new QLabel(this);
    penwidthlbl->setText("Pen width");
    penwidthlbl->setStyleSheet("font-size: 20pt;");

    ui->toolBar2->addWidget(penwidthlbl);



    QSpinBox *spinBox = new QSpinBox(this);
    ui->toolBar2->addWidget(spinBox);
    spinBox->setRange(1,50);
    spinBox->connect(spinBox, QOverload<int>::of(&QSpinBox::valueChanged), [spinBox ,this]()
                        {
                scribbleArea->setPenWidth(spinBox->value());
            }
            );




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
    if (scribbleArea->isModified() && !scribbleArea->undoStack.empty()) {
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
    scribbleArea->setTool("circle");
    ui->actionCursor->setChecked(false);
    ui->actionSquare->setChecked(false);
    ui->actionLine->setChecked(false);
    ui->actionFill->setChecked(false);
    ui->actionpencil->setChecked(false);
       ui->actioneraser->setChecked(false);
        this->setCursor(Qt::CrossCursor);

}


void MainWindow::on_actionCursor_triggered()
{
    ui->actionCircle->setChecked(false);
    ui->actionSquare->setChecked(false);
    ui->actionLine->setChecked(false);
    ui->actionFill->setChecked(false);
    ui->actionpencil->setChecked(false);
    ui->actioneraser->setChecked(false);
    scribbleArea->setTool("None");
    this->setCursor(Qt::ArrowCursor);

}


void MainWindow::on_actionLine_triggered()
{
    ui->actionpencil->setChecked(false);
    ui->actionCursor->setChecked(false);
    ui->actionSquare->setChecked(false);
    ui->actionCircle->setChecked(false);
    ui->actionFill->setChecked(false);
       ui->actioneraser->setChecked(false);
    scribbleArea->setTool("line");
    this->setCursor(Qt::CrossCursor);


}


void MainWindow::on_actionFill_triggered()
{
    ui->actionpencil->setChecked(false);
    ui->actionCursor->setChecked(false);
    ui->actionSquare->setChecked(false);
    ui->actionCircle->setChecked(false);
    ui->actionLine->setChecked(false);
       ui->actioneraser->setChecked(false);
    scribbleArea->setTool("fill");

    QPixmap p = QPixmap(":/img/paint-bucket.png");
   QPixmap scaled = p.scaled(QSize(50, 50));
    QCursor c = QCursor(scaled, 0, 0);
    this->setCursor(c);
}


void MainWindow::on_actionSquare_triggered()
{
    ui->actionCursor->setChecked(false);
    ui->actionFill->setChecked(false);
    ui->actionCircle->setChecked(false);
    ui->actionLine->setChecked(false);
    ui->actionpencil->setChecked(false);
       ui->actioneraser->setChecked(false);
    scribbleArea->setTool("rect");
    this->setCursor(Qt::CrossCursor);
}

void MainWindow::on_actionpencil_triggered()
{
    ui->actionCircle->setChecked(false);
    ui->actionSquare->setChecked(false);
    ui->actionLine->setChecked(false);
    ui->actionFill->setChecked(false);
    ui->actionCursor->setChecked(false);
    ui->actioneraser->setChecked(false);
    scribbleArea->setTool("free");

    this->setCursor(Qt::CrossCursor);
}

void MainWindow::on_actioneraser_triggered()
{
    ui->actionCircle->setChecked(false);
    ui->actionSquare->setChecked(false);
    ui->actionLine->setChecked(false);
    ui->actionFill->setChecked(false);
    ui->actionCursor->setChecked(false);
    ui->actionpencil->setChecked(false);
    scribbleArea->setTool("eraser");

    QPixmap p = QPixmap(":/img/eraser.png");
   QPixmap scaled = p.scaled(QSize(40, 40));
    QCursor c = QCursor(scaled, 0, 0);
    this->setCursor(c);
}

void MainWindow::on_actioncolorPicker_triggered()
{
    if (scribbleArea->picker){
        ui->toolBar2->hide();
        scribbleArea->setpicker(false);
    }else{
        ui->toolBar2->show();
        scribbleArea->setpicker(true);
    }
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
   scribbleArea->clearImage();
   on_actionCursor_triggered();
}





void MainWindow::on_actionUndo_triggered()
{
    if (!scribbleArea->undoStack.empty()){
        scribbleArea->undo();
         on_actionCursor_triggered();
     }
}


void MainWindow::on_actionredo_triggered()
{
    if (!scribbleArea->redoStack.empty()){
        scribbleArea->redo();
         on_actionCursor_triggered();

}
}


void MainWindow::on_actionAbout_Project_triggered()
{
    QMessageBox::about(this, tr("About Decolor"),
            tr("<p>The <b>Decolor </b> is a data Structures Project "
               "An MS paint like application which allows you to draw Rectangles, "
               "Circle and various shapes, In addition to free hand drawing, packed with features like flood fill and and choosing colors and erasing. "
               ));
}


void MainWindow::on_actionPreferences_triggered()
{
    QStringList items;
       items << tr("Light Mode") << tr("Dark Mode") ;



       bool ok1;
       QInputDialog s ;
       s.setStyleSheet("color : #000000 ;");
       QString item = s.getItem(this, tr("Preferences"), tr("Theme:"), items, 0, false, &ok1);



       if (ok1 && !item.isEmpty()){
           if (item =="Dark Mode"){
               this->scribbleArea->setmode(true);
               ui->toolBar1->setStyleSheet("background-color: #3B3C36 ; color :white ;");
               ui->toolBar2->setStyleSheet(" background-color: #3B3C36 ;  color: white ; ");
                ui->menubar1->setStyleSheet("QMenuBar {  background-color: #3B3C36 ; color : white; }");

           }else if  (item == "Light Mode"){
                  this->scribbleArea->setmode(false);
               ui->menubar1->setStyleSheet("background-color:  #F0F1F3 ;color : black;");
               ui->toolBar1->setStyleSheet("background-color:  #F0F1F3 ; color: black ; ");
               ui->toolBar2->setStyleSheet("background-color:  #F0F1F3 ; color: black ;");
           }
              }


}

