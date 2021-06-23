#include "imageproc.h"
#include "ui_imageproc.h"

ImageProc::ImageProc(cv::Mat &lframe, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageProc)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView->scene()->addItem(&pix);
    frame = lframe;

}

QImage ImageProc::cvMatToQImage(const cv::Mat &inMat)
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

QPixmap ImageProc::cvMatToQPixmap(const cv::Mat &inMat)
{
    return QPixmap::fromImage( cvMatToQImage( inMat ) );
}

cv::Mat ImageProc::prepocessing(cv::Mat &img)
{
    cv::Mat imGray, imBlur, imCanny, imDil, imErode;
    cvtColor(img, imGray, cv::COLOR_BGR2GRAY);
    GaussianBlur(imGray, imBlur, cv::Size(3,3), 3, 0);
    Canny(imBlur, imCanny, 25, 75);
    cv::Mat kernel = getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
    cv::dilate(imCanny, imDil, kernel);
    cv::erode(imDil, imErode, kernel);
    return imErode;
}

std::vector<cv::Point> ImageProc::getCountors(cv::Mat &imgD)
{
    using namespace std;
    using namespace cv;

    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    findContours(imgD, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);


    vector<vector<Point>>conPoly(contours.size());

    vector<Point> biggest;
    int maxArea = 0;
    for(size_t i = 0; i < contours.size(); i++){
        int area = contourArea(contours[i]);

        if(area > 1000){
            float peri = arcLength(contours[i], true);
            approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

            if(area > maxArea && conPoly[i].size() == 4){
                //drawContours(imgRotate, conPoly, i, Scalar(255,0,255), 5);
                biggest = {conPoly[i][0], conPoly[i][1], conPoly[i][2], conPoly[i][3]};
                maxArea = area;
            }

            //rectangle(imgD, boundRect[i].tl(),boundRect[i].br(), Scalar(0,255,0), 5);
        }
    }
    return biggest;
}

std::vector<cv::Point> ImageProc::reorderPoints(std::vector<cv::Point> points)
{
    std::vector<cv::Point> newPoints;
    std::vector<int> sumPoints, subPoints;
    for(size_t i = 0; i < points.size(); i++){
        sumPoints.push_back( points[i].x + points[i].y);
        subPoints.push_back( points[i].x - points[i].y);
    }
    newPoints.push_back(points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);
    newPoints.push_back(points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
    newPoints.push_back(points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
    newPoints.push_back(points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);

    return newPoints;
}

cv::Mat ImageProc::getWarp(cv::Mat img, std::vector<cv::Point> points, float w, float h)
{
    cv::Mat imgWarp;
    cv::Point2f src[4] = {points[0], points[1],points[2], points[3]};
    cv::Point2f dst[4] = {{0.0f, 0.0f}, {w,0.0f}, {0.0f, h}, {w,h}};
    cv::Mat matrix = cv::getPerspectiveTransform(src, dst);
    cv::warpPerspective(img, imgWarp, matrix, cv::Point(w,h), cv::INTER_NEAREST);

    return imgWarp;
}

ImageProc::~ImageProc()
{
    delete ui;
}

void ImageProc::on_button_rotate_left_clicked()
{
    cv::Mat imRotate;
    cv::rotate(frame, imRotate, cv::ROTATE_90_COUNTERCLOCKWISE);
    frame = imRotate;
    pix.setPixmap(cvMatToQPixmap(frame));
}

void ImageProc::on_button_rotate_right_clicked()
{
    cv::Mat imRotate;
    cv::rotate(frame, imRotate, cv::ROTATE_90_CLOCKWISE);
    frame = imRotate;
    pix.setPixmap(cvMatToQPixmap(frame));
}

void ImageProc::on_button_crop_clicked()
{
    preProcFrame = prepocessing(frame);
    initialPoints = getCountors(preProcFrame);
    docPoint = reorderPoints(initialPoints);
    imgWarped = getWarp(frame, docPoint, 1260.f, 1800.f);
    docWork workplace(imgWarped);
    workplace.exec();

}

void ImageProc::on_button_close_window_clicked()
{
    cv::destroyAllWindows();
    ImageProc::close();
}

void ImageProc::on_button_load_clicked()
{
    if(!frame.empty()){
        pix.setPixmap(cvMatToQPixmap(frame));
        ui->graphicsView->fitInView(&pix, Qt::KeepAspectRatio);
    }
    qApp->processEvents();
}

void ImageProc::on_button_read_clicked()
{
    docWork work(frame);
    work.exec();
}
