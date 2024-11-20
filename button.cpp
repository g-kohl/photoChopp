#include "button.hpp"

using namespace std;
using namespace cv;
using namespace QtCharts;

// files
void loadImage(QLabel *loadedImageLabel, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage){
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Load image", "", "Images (*.png *.jpg *.bmp)");
    string imageName = fileName.toStdString();
    Mat matImage = imread(imageName, IMREAD_COLOR);

    if(matImage.empty()){
        cout << "Error: empty image";
        return;
    }

    loadedImage.setImage(matImage);
    loadedImage.setName(imageName);
    loadedImage.setSize(matImage.rows, matImage.cols);

    savedImage.setImage(matImage);
    savedImage.setSize(matImage.rows, matImage.cols);

    if(!fileName.isEmpty()){
        QPixmap image(fileName);
        loadedImageLabel->setAlignment(Qt::AlignCenter);
        loadedImageLabel->setPixmap(image.scaled(loadedImageLabel->size(), Qt::KeepAspectRatio));
        processedImageLabel->setAlignment(Qt::AlignCenter);
        processedImageLabel->setPixmap(image.scaled(processedImageLabel->size(), Qt::KeepAspectRatio));
    }
}

void saveImage(QWidget *parent, QLabel *processedImageLabel, Image &savedImage){
    bool ok;
    QString text = QInputDialog::getText(parent, "Save Image", "Enter the name of the file (with extension)", QLineEdit::Normal, "", &ok);

    if(!ok)
        return;

    string fileName = text.toStdString();

    savedImage.setName(fileName);
    savedImage.saveImage();
}

// options
void restoreImage(QLabel *processedImageLabel, Image &loadedImage, Image &savedImage){
    savedImage.setImage(loadedImage.getImage());
    savedImage.setSize(loadedImage.getSize().first, loadedImage.getSize().second);

    QImage imgQ(savedImage.getImage().data, savedImage.getImage().cols, savedImage.getImage().rows, savedImage.getImage().step, QImage::Format_RGB888);
    QPixmap image = QPixmap::fromImage(imgQ.rgbSwapped());

    processedImageLabel->setAlignment(Qt::AlignCenter);
    processedImageLabel->setPixmap(image.scaled(processedImageLabel->size(), Qt::KeepAspectRatio));
}

void loadNextImage(QLabel *loadedImageLabel, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage){
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Load source image", "", "Images (*.png *.jpg *.bmp)");
    string imageName = fileName.toStdString();
    Mat matImage = imread(imageName, IMREAD_COLOR);

    if(matImage.empty()){
        cout << "Error: empty image";
        return;
    }

    loadedImage.RGBtoGrayscale();

    savedImage.setImage(matImage);
    savedImage.setSize(matImage.rows, matImage.cols);
    savedImage.RGBtoGrayscale();

    displayImage(loadedImageLabel, loadedImage, true);
    displayImage(processedImageLabel, savedImage, true);
}

// basic processing
void applyRGBtoGrayscale(QLabel *processedImageLabel, Image &savedImage){
    savedImage.RGBtoGrayscale();

    displayImage(processedImageLabel, savedImage, true);
}

void applyQuantization(QWidget *parent, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage){
    bool ok;
    int shades = QInputDialog::getInt(parent, "Quantization", "Enter the number of shades:", 0, -2147483647, 2147483647, 1, &ok);

    if(!ok)
        return;    

    savedImage.setImage(loadedImage.getImage());
    savedImage.RGBtoGrayscale();
    savedImage.quantization(shades);

    displayImage(processedImageLabel, savedImage, true);
}

void applyAdjustBrightness(QWidget *parent, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage){
    bool ok;
    int delta = QInputDialog::getInt(parent, "Adjust Brightness", "Enter the brightness variation:", 0, -2147483647, 2147483647, 1, &ok);

    if(!ok)
        return;

    savedImage.setImage(loadedImage.getImage());
    savedImage.adjustBrightness(delta);

    displayImage(processedImageLabel, savedImage, true);
}

void applyAdjustContrast(QWidget *parent, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage){
    bool ok;
    int delta = QInputDialog::getInt(parent, "Adjust Contrast", "Enter the contrast variation:", 0, -2147483647, 2147483647, 1, &ok);

    if(!ok)
        return;

    savedImage.setImage(loadedImage.getImage());
    savedImage.adjustContrast(delta);

    displayImage(processedImageLabel, savedImage, true);
}

void applyNegative(QLabel *processedImageLabel, Image &savedImage){
    savedImage.negative();
    
    displayImage(processedImageLabel, savedImage, true);
}

// change formatting
void applyInvertVertical(QLabel *processedImageLabel, Image &savedImage){
    savedImage.invertVertical();

    displayImage(processedImageLabel, savedImage, true);
}

void applyInvertHorizontal(QLabel *processedImageLabel, Image &savedImage){
    savedImage.invertHorizontal();
    
    displayImage(processedImageLabel, savedImage, true);
}

void applyRotate(QWidget *parent, QLabel *processedImageLabel, Image &savedImage){
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(parent, "Rotate", "Clockwise (Yes) or Counter-clockwise? (No)", QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes){
        savedImage.rotate(true);
    }
    else{
        savedImage.rotate(false);
    }
    
    displayImage(processedImageLabel, savedImage, true);
}

void applyZoomOut(QWidget *parent, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage){
    bool ok_sy, ok_sx;
    int sy, sx;

    sy = QInputDialog::getInt(parent, "Reduction Factor", "Enter the vertical factor:", 0, -2147483647, 2147483647, 1, &ok_sy);
    
    if(ok_sy){
        sx = QInputDialog::getInt(parent, "Reduction Factor", "Enter the horizontal factor:", 0, -2147483647, 2147483647, 1, &ok_sx);
    }

    if(!ok_sy || !ok_sx)
        return;

    savedImage.setImage(loadedImage.getImage());
    savedImage.setSize(loadedImage.getSize().first, loadedImage.getSize().second);
    savedImage.zoomOut(sy, sx);

    displayImage(processedImageLabel, savedImage, false);
}

void applyZoomIn(QWidget *parent, QLabel *processedImageLabel, Image &savedImage){
    bool ok;
    int zoomQuantity = QInputDialog::getInt(parent, "Zoom In", "Enter zooms quantity:", 0, -2147483647, 2147483647, 1, &ok);

    if(!ok)
        return;

    Image zoomedImage(savedImage);

    for(int i=0; i<zoomQuantity; i++)
        zoomedImage.zoomIn();

    QImage imgQ(zoomedImage.getImage().data, zoomedImage.getImage().cols, zoomedImage.getImage().rows, zoomedImage.getImage().step, QImage::Format_RGB888);
    QPixmap image = QPixmap::fromImage(imgQ.rgbSwapped());

    QDialog *zoomedWindow = new QDialog;
    zoomedWindow->setWindowTitle("Zoomed In Image");

    QLabel *imageLabel = new QLabel;
    imageLabel->setFixedSize(image.size());
    imageLabel->setPixmap(image);
    
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(imageLabel);
    zoomedWindow->setLayout(layout);
    zoomedWindow->exec();
}

// convolution
void applyConvolution(QLabel *processedImageLabel, Image &savedImage, std::vector<float> kernel, bool grayscale, bool clamping){
    if(grayscale)
        savedImage.RGBtoGrayscale();

    savedImage.convolution(kernel, clamping);

    displayImage(processedImageLabel, savedImage, true);
}

// histogram
void showHistogram(QLabel *processedImageLabel, Image &savedImage){
    savedImage.RGBtoGrayscale();
    displayImage(processedImageLabel, savedImage, true);
    QVector<int> shades = savedImage.histogram();

    QBarSet *barSet = new QBarSet("Shades");

    for(int value : shades){
        *barSet << value;
    }

    barSet->setColor(QColor("black"));

    QBarSeries *series = new QBarSeries();
    series->append(barSet);
    series->setBarWidth(2.0);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Histogram");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, shades.size()-1);
    axisX->setTitleText("Shades");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, *std::max_element(shades.begin(), shades.end()));
    axisY->setTitleText("Frequency");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QWidget *histogramWindow = new QWidget;
    histogramWindow->setWindowTitle("Histogram");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(chartView);
    histogramWindow->setLayout(layout);
    histogramWindow->resize(800, 600);
    histogramWindow->show();
}

void applyEqualization(QLabel *processedImageLabel, Image &savedImage){
    savedImage.RGBtoGrayscale();
    savedImage.equalization();
    
    displayImage(processedImageLabel, savedImage, true);
}

void applyHistogramMatching(QLabel *processedImageLabel, Image &loadedImage, Image &savedImage){
    savedImage.matching(loadedImage);

    displayImage(processedImageLabel, savedImage, true);
}


// auxiliary
void displayImage(QLabel *imageLabel, Image &displayedImage, bool scale){
    QImage imgQ(displayedImage.getImage().data, displayedImage.getImage().cols, displayedImage.getImage().rows, displayedImage.getImage().step, QImage::Format_RGB888);
    QPixmap image = QPixmap::fromImage(imgQ.rgbSwapped());

    imageLabel->setAlignment(Qt::AlignCenter);

    if(scale){
        imageLabel->setPixmap(image.scaled(imageLabel->size(), Qt::KeepAspectRatio));
    }
    else{
        imageLabel->setPixmap(image);
    }
}

vector<float> readInputs(QVector<QLineEdit*> convolutionInputs){
    vector<float> kernel;

    for(int i=0; i<INPUTS_NUM; i++){
        kernel.push_back(convolutionInputs[i]->text().toFloat());
    }

    return kernel;
}