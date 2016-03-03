#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QShortcut *shortcut = new QShortcut(QKeySequence("SPACE"), this);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(on_btnForceDet_clicked()));

   // QObject::connect(shortcut, SIGNAL(activated()), receiver, SLOT(yourSlotHere()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::getSliderValue() const
{
    return ui->sldThresohld->value();
}
int MainWindow::getCheckBoxValue() const
{
    return ui->ckbForceSignal->isChecked();
}

int MainWindow::getForceDetection() const
{
   return forceDetection;
}

int MainWindow::setForceDetection(bool value)
{
    forceDetection=value;
}

void MainWindow::on_btnForceDet_clicked()
{
    forceDetection=1;
}

void MainWindow::on_btnQuit_clicked()
{
    close();
}

void MainWindow::setText(QString text, int flagAppend=0)
{
    if (flagAppend==1)
        ui->txtInfo->setText(this->getText() + text);
    else
    ui->txtInfo->setText(text);
}

QString MainWindow::getText() const
{
    ui->txtInfo->toPlainText();
}
