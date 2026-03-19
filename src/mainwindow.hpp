#pragma once

#include "camera.hpp"
#include <iostream>
#include <QFileDialog>
#include <QMainWindow>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/core/mat.hpp>
#include <tesseract/baseapi.h>

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
