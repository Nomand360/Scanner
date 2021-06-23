#ifndef DOCWORK_H
#define DOCWORK_H

#include <QDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QFileDialog>
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

namespace Ui {
class docWork;
}

class docWork : public QDialog
{
    Q_OBJECT

public:
    explicit docWork(cv::Mat &img,QWidget *parent = nullptr);
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

#endif // DOCWORK_H
