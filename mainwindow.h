#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <camera.h>

#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    cv::Mat QImageToMat(const QImage &image);
    ~MainWindow();

private slots:
    void on_button_Exit_clicked();

    void on_button_Camera_clicked();

    void on_button_LoadFile_clicked();

private:
    Ui::MainWindow *ui;
    QImage img;
    cv::Mat myImg;
};
#endif // MAINWINDOW_H
