#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class Canvas;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


protected:
    void closeEvent(QCloseEvent *event) override;
private slots:

   bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);

    void on_actionCircle_triggered();

    void on_actionCursor_triggered();

    void on_actionLine_triggered();

    void on_actionFill_triggered();

    void on_actionSquare_triggered();



    void on_actionOpen_triggered();

    void on_actionSave_as_triggered();


    void on_actionExit_triggered();

    void on_actionPen_Color_triggered();

    void on_actionPen_Width_triggered();

    void on_actionClear_Screen_triggered();


    void on_actionpencil_triggered();

    void on_actionUndo_triggered();

    void on_actionredo_triggered();

    void on_actionAbout_Project_triggered();

    void on_actionPreferences_triggered();

    void on_actioneraser_triggered();

    void on_actioncolorPicker_triggered();

    void on_actioncolorfinder_triggered();

private:

    Ui::MainWindow *ui;

    Canvas *canvas;
};
#endif // MAINWINDOW_H
