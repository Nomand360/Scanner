#include "docwork.h"
#include "ui_docwork.h"

docWork::docWork(cv::Mat &img, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::docWork)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView->scene()->addItem(&pixmap);
    imgF = img;
    ocr = new tesseract::TessBaseAPI;
    eng = "eng";
    rus = "rus";

}

QImage docWork::cvMatToQImage(const cv::Mat &inMat)
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

QPixmap docWork::cvMatToQPixmap(const cv::Mat &inMat)
{
    return QPixmap::fromImage( cvMatToQImage( inMat ) );
}

void docWork::saveTofile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save image"), "image.png", tr("image(*.png *.jpeg)"));
    QImage qimg(fileName);
    qimg = cvMatToQImage(imgF);
    qimg.save(fileName);
}

docWork::~docWork()
{
    delete ui;
}

void docWork::on_button_contrast_clicked()
{
    cv::Mat sharp;
    cv::Mat sharpening_kernel = (cv::Mat_<double>(3, 3) << -1, -1, -1,
                             -1, 9, -1,
                             -1, -1, -1);
    filter2D(imgF, sharp, -1, sharpening_kernel);
    imgF = sharp;
    pixmap.setPixmap(cvMatToQPixmap(imgF));
}

void docWork::on_button_read_text_clicked()
{
    if(ui->language_box->currentIndex() == 0){
        lang = &eng[0];
    }else if(ui->language_box->currentIndex() == 1){
        lang = &rus[0];
    }
    ocr->Init(NULL, lang, tesseract::OEM_LSTM_ONLY);

    ocr->SetPageSegMode(tesseract::PSM_AUTO);
    ocr->SetVariable("user_defined_dpi_", "70");
    ocr->SetImage(imgF.data, imgF.cols, imgF.rows,3, imgF.step);
    ocr->TessBaseAPI::SetSourceResolution(125);
    QString str2(ocr->GetUTF8Text());
    ocr->End();
    QString filename = QFileDialog::getSaveFileName(this, tr("Save text"), "text.txt", tr("text(*.txt)"));
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&file);
        out << str2;
    }
    file.close();
}

void docWork::on_button_save_clicked()
{
    saveTofile();
}

void docWork::on_button_close_clicked()
{
    cv::destroyAllWindows();
    docWork::close();
}

void docWork::on_button_load_clicked()
{
    if(!imgF.empty()){
        pixmap.setPixmap(cvMatToQPixmap(imgF));
        ui->graphicsView->fitInView(&pixmap, Qt::KeepAspectRatio);
    }
    qApp->processEvents();
}
