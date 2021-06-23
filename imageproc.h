#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <docwork.h>

namespace Ui {
class ImageProc;
}

class ImageProc : public QDialog
{
    Q_OBJECT

public:
    explicit ImageProc(cv::Mat &lframe, QWidget *parent = nullptr);
    QImage cvMatToQImage(const cv::Mat &inMat);
    QPixmap cvMatToQPixmap(const cv::Mat &inMat);
    cv::Mat prepocessing(cv::Mat &img);
    std::vector<cv::Point> getCountors(cv::Mat &img);
    std::vector<cv::Point> reorderPoints(std::vector<cv::Point> points);
    cv::Mat getWarp(cv::Mat img, std::vector<cv::Point> points, float w, float h);

    ~ImageProc();

private slots:
    void on_button_rotate_left_clicked();

    void on_button_rotate_right_clicked();

    void on_button_crop_clicked();

    void on_button_close_window_clicked();

    void on_button_load_clicked();

    void on_button_read_clicked();

private:
    Ui::ImageProc *ui;
    QGraphicsPixmapItem pix;
    cv::Mat frame, preProcFrame, imgWarped;
    std::vector<cv::Point> initialPoints, docPoint;
};

#endif // IMAGEPROC_H
