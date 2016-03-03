#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
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
    int forceDetection=0;

    void setText(QString, int);
    QString getText() const;
public slots:
    int setForceDetection(bool);

private slots:
    void on_btnForceDet_clicked();

    void on_btnQuit_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
