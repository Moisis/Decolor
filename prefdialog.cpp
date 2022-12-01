#include "prefdialog.h"
#include "ui_prefdialog.h"

#include "mainwindow.h"
#include "scribblearea.h"



PrefDialog::PrefDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrefDialog)
{
    ui->setupUi(this);
}

PrefDialog::~PrefDialog()
{
    delete ui;
}

void PrefDialog::on_buttonBox_accepted()
{
MainWindo

}

void PrefDialog::on_buttonBox_rejected()
{

}

