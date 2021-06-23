#include "camera.h"
#include "ui_camera.h"

Camera::Camera(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Camera)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView->scene()->addItem(&pixmap);
    ui->button_Scan->setEnabled(false);

}

QImage Camera::cvMatToQImage(const cv::Mat &inMat)
{
    switch ( inMat.type() )
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_ARGB32 );

        return image;
    }

        // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_RGB888 );

        return image.rgbSwapped();
    }

        // 8-bit, 1 channel
    case CV_8UC1:
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_Grayscale8 );
#else
        static QVector<QRgb>  sColorTable;

        // only create our color table the first time
        if ( sColorTable.isEmpty() )
        {
            sColorTable.resize( 256 );

            for ( int i = 0; i < 256; ++i )
            {
                sColorTable[i] = qRgb( i, i, i );
            }
        }

        QImage image( inMat.data,
                      inMat.cols, inMat.rows,
                      static_cast<int>(inMat.step),
                      QImage::Format_Indexed8 );

        image.setColorTable( sColorTable );
#endif

        return image;
    }

    default:
        qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
        break;
    }

    return QImage();
}

QPixmap Camera::cvMatToQPixmap(const cv::Mat &inMat)
{
    return QPixmap::fromImage( cvMatToQImage( inMat ) );
}

Camera::~Camera()
{
    delete ui;
}

void Camera::on_button_Start_clicked()
{
    using namespace cv;
    ui->button_Scan->setEnabled(true);
    int cameraIndex = ui->lineEdit->text().toInt();
    video.open(cameraIndex);
    video.set(CAP_PROP_FPS, 30);
    while(video.isOpened()){
        video >> frame;
        if(!frame.empty()){
            //QImage qimg(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
            //pixmap.setPixmap(QPixmap::fromImage(qimg.rgbSwapped()));
            pixmap.setPixmap(cvMatToQPixmap(frame));
            ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
        }
        qApp->processEvents();
    }
}

void Camera::on_button_exit_clicked()
{
    video.release();
    cv::destroyAllWindows();
    Camera::close();
}

void Camera::on_button_Scan_clicked()
{
    if(video.isOpened()){
        cloneFrame = frame.clone();
    }
    ImageProc image(cloneFrame);
    image.exec();
}
