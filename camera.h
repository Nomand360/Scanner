#ifndef CAMERA_H
#define CAMERA_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <imageproc.h>

namespace Ui {
class Camera;
}

class Camera : public QDialog
{
    Q_OBJECT

public:
    explicit Camera(QWidget *parent = nullptr);
    QImage cvMatToQImage(const cv::Mat &inMat);
    QPixmap cvMatToQPixmap(const cv::Mat &inMat);
    ~Camera();

private slots:
    void on_button_Start_clicked();

    void on_button_exit_clicked();

    void on_button_Scan_clicked();

private:
    Ui::Camera *ui;
    QGraphicsPixmapItem pixmap;
    cv::VideoCapture video;
    cv::Mat frame, cloneFrame;
};

#endif // CAMERA_H
