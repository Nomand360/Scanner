#pragma once

#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QFileDialog>
#include <tesseract/baseapi.h>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/core/mat.hpp>
#include "imageproc.hpp"


namespace Ui {
class docWork;
}

class docWork : public QDialog
{
    Q_OBJECT

public:
    explicit docWork(const cv::Mat &img ,QWidget *parent = nullptr);
    QImage cvMatToQImage(const cv::Mat &inMat);
    QPixmap cvMatToQPixmap(const cv::Mat &inMat);
    void saveTofile();
    ~docWork();

private slots:
    void on_button_contrast_clicked();

    void on_button_read_text_clicked();

    void on_button_save_clicked();

    void on_button_close_clicked();

    void on_button_load_clicked();

private:

    Ui::docWork *ui;
    QGraphicsPixmapItem pixmap;
    cv::Mat imgF;
    tesseract::TessBaseAPI *ocr;
    std::string eng, rus;
    char *lang;
};
