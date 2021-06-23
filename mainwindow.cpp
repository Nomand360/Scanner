#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

cv::Mat MainWindow::QImageToMat(const QImage &image)
{
    cv::Mat out;
        switch(image.format()) {
        case QImage::Format_Invalid:
        {
            cv::Mat empty;
            empty.copyTo(out);
            break;
        }
        case QImage::Format_RGB32:
        {
            cv::Mat view(image.height(),image.width(),CV_8UC4,(void *)image.constBits(),image.bytesPerLine());
            view.copyTo(out);
            break;
        }
        case QImage::Format_RGB888:
        {
            cv::Mat view(image.height(),image.width(),CV_8UC3,(void *)image.constBits(),image.bytesPerLine());
            cv::cvtColor(view, out, cv::COLOR_RGB2BGR);
            break;
        }
        default:
        {
            QImage conv = image.convertToFormat(QImage::Format_ARGB32);
            cv::Mat view(conv.height(),conv.width(),CV_8UC4,(void *)conv.constBits(),conv.bytesPerLine());
            view.copyTo(out);
            break;
        }
        }
        return out;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_button_Exit_clicked()
{
    MainWindow::close();
}

void MainWindow::on_button_Camera_clicked()
{
    Camera cam;
    cam.exec();
}

void MainWindow::on_button_LoadFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load image"), "", tr("image(*.png *.jpeg *.JPG *jpg)"));
    if(!fileName.isEmpty()){
        std::string file =
        fileName.toStdString();
        cv::Mat myImg = cv::imread(file);
        cv::Mat res;
        cv::resize(myImg, res, cv::Size(0, 0), 0.5, 0.5, cv::INTER_NEAREST);
        //cv::imwrite("sheet.jpeg", res);
        //cv::imshow("img", res);
        ImageProc proc(res);
        proc.exec();
    }else{
        std::cout << "error" <<std::endl;
    }
}
