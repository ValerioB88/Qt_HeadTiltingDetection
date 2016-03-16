#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMessageBox>
#include <QMainWindow>
#include <QString>
#include <QShortcut>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int getSliderValue() const;
    int getCheckBoxValue() const;
    int getForceDetection() const;
    int isPaused() const;

    void setSliderValue(int);
    void callInstructions();
    void rotateRight();
    void rotateLeft();
    void flipHorz();
    void flipVert();

    int forceDetection=0;
    int rotate=0;

    int flipHorzValue=0;
    int flipVertValue=0;
public slots:
    void setForceDetection(bool);

private slots:
    void on_btnForceDet_clicked();

    void on_btnQuit_clicked();

    void on_actionInstructions_triggered();


    void on_btnFlipR_clicked();

    void on_btnFlipL_clicked();



    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
