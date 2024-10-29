#include "button.hpp"

using namespace std;
using namespace cv;

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
    savedImage.setName(imageName);
    savedImage.setSize(matImage.rows, matImage.cols);

    if(!fileName.isEmpty()){
        QPixmap image(fileName);
        loadedImageLabel->setPixmap(image.scaled(loadedImageLabel->size(), Qt::KeepAspectRatio));
        processedImageLabel->setPixmap(image.scaled(loadedImageLabel->size(), Qt::KeepAspectRatio));
    }
}

void saveImage(QWidget *parent, QLabel *imageLabel, Image &savedImage){
    bool ok;
    QString text = QInputDialog::getText(parent, "Save Image", "Enter the name of the file (with extension)", QLineEdit::Normal, "", &ok);
    string fileName = text.toStdString();

    savedImage.setName(fileName);
    savedImage.saveImage();
}

void flipVertical(QLabel *imageLabel, Image &loadedImage, Image &savedImage){
    savedImage.invertImageVertical();

    QImage imgQ(savedImage.getImage().data, savedImage.getImage().cols, savedImage.getImage().rows, savedImage.getImage().step, QImage::Format_RGB888);
    QPixmap image = QPixmap::fromImage(imgQ.rgbSwapped());

    imageLabel->setPixmap(image.scaled(imageLabel->size(), Qt::KeepAspectRatio));
}

void flipHorizontal(QLabel *imageLabel, Image &loadedImage, Image &savedImage){
    savedImage.invertImageHorizontal();
    
    QImage imgQ(savedImage.getImage().data, savedImage.getImage().cols, savedImage.getImage().rows, savedImage.getImage().step, QImage::Format_RGB888);
    QPixmap image = QPixmap::fromImage(imgQ.rgbSwapped());

    imageLabel->setPixmap(image.scaled(imageLabel->size(), Qt::KeepAspectRatio));
}

void applyGrayscale(QLabel *imageLabel, Image &loadedImage, Image &savedImage){
    savedImage.RGBtoGrayscale();

    QImage imgQ(savedImage.getImage().data, savedImage.getImage().cols, savedImage.getImage().rows, savedImage.getImage().step, QImage::Format_RGB888);
    QPixmap image = QPixmap::fromImage(imgQ.rgbSwapped());

    imageLabel->setPixmap(image.scaled(imageLabel->size(), Qt::KeepAspectRatio));
}

void applyQuantization(QWidget *parent, QLabel *imageLabel, Image &loadedImage, Image &savedImage){
    bool ok;
    int shades = QInputDialog::getInt(parent, "Quantization", "Enter the number of shades:", 0, -2147483647, 2147483647, 1, &ok);

    savedImage.setImage(loadedImage.getImage());
    savedImage.RGBtoGrayscale();
    savedImage.quantization(shades);

    QImage imgQ(savedImage.getImage().data, savedImage.getImage().cols, savedImage.getImage().rows, savedImage.getImage().step, QImage::Format_RGB888);
    QPixmap image = QPixmap::fromImage(imgQ.rgbSwapped());

    imageLabel->setPixmap(image.scaled(imageLabel->size(), Qt::KeepAspectRatio));
}
