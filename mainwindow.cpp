#include "mainwindow.h"
#include "ui_mainwindow.h"


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



void MainWindow::setForceDetection(bool value)
{
    forceDetection=value;
}

void MainWindow::on_btnForceDet_clicked()
{
    forceDetection=1;
}


void MainWindow::setSliderValue(int value)
{
    ui->sldThresohld->setValue(value);
}

void MainWindow::on_btnQuit_clicked()
{
    close();
}

int MainWindow::isPaused() const
{
    return ui->btnPause->isChecked();
}

void MainWindow::rotateRight()
{
    if (++rotate>3) rotate=0;
}

void MainWindow::rotateLeft()
{
    if (--rotate<0) rotate=3;
}

void MainWindow::on_actionInstructions_triggered()
{
    this->callInstructions();
}

void MainWindow::callInstructions()
{
    ; //QMessageBox::about(this, "Instructions", "HEAD TILTING DETECTION/AUTOMATIC PAGE TURNER FOR PIANISTS\n-If you have glasses, take it off. This software does not work with glasses yet. \n-Wait for the software to detect your eyes. After few seconds, a red rectangle should appear around each of your eye. \n-Move your head on the right until you hear a sound/see a green circle on the same direction of your movement. This will trigger the action. After the action is triggered (page up/down), you can move your head in the initial position.\n-The action will be triggered when Rotation Value reaches ""Rot. Value Threshold"". If you want you can play around with this setting.\nValerio Biscione 2016\nvaleriobiscione.com");

}

void MainWindow::flipHorz()
{
    if (this->flipHorzValue)
        this->flipHorzValue=0;
    else
        this->flipHorzValue=1;
}

void MainWindow::flipVert()
{
    if (this->flipVertValue)
        this->flipVertValue=0;
    else
        this->flipVertValue=1;

}

void MainWindow::on_btnFlipR_clicked()
{
    this->rotateRight();
}

void MainWindow::on_btnFlipL_clicked()
{
    this->rotateLeft();
}



void MainWindow::on_pushButton_2_clicked()
{
    this->flipVert();
}

void MainWindow::on_pushButton_clicked()
{
    this->flipHorz();
}
