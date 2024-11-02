#include "button.hpp"

using namespace std;
using namespace cv;

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
        processedImageLabel->setPixmap(image.scaled(loadedImageLabel->size(), Qt::KeepAspectRatio));
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

void generateHistogram(QWidget *parent, QLabel *imageLabel, Image &savedImage){
    QVector<int> shades = savedImage.histogram();
}

// basic processing
void applyRGBtoGrayscale(QLabel *processedImageLabel, Image &savedImage){
    savedImage.RGBtoGrayscale();

    displayImage(processedImageLabel, savedImage);
}

void applyQuantization(QWidget *parent, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage){
    bool ok;
    int shades = QInputDialog::getInt(parent, "Quantization", "Enter the number of shades:", 0, -2147483647, 2147483647, 1, &ok);

    if(!ok)
        return;    

    savedImage.setImage(loadedImage.getImage());
    savedImage.RGBtoGrayscale();
    savedImage.quantization(shades);

    displayImage(processedImageLabel, savedImage);
}

void applyAdjustBrightness(QWidget *parent, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage){
    bool ok;
    int delta = QInputDialog::getInt(parent, "Adjust Brightness", "Enter the brightness variation:", 0, -2147483647, 2147483647, 1, &ok);

    if(!ok)
        return;

    savedImage.setImage(loadedImage.getImage());
    savedImage.adjustBrightness(delta);

    displayImage(processedImageLabel, savedImage);
}

void applyAdjustContrast(QWidget *parent, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage){
    bool ok;
    int delta = QInputDialog::getInt(parent, "Adjust Contrast", "Enter the contrast variation:", 0, -2147483647, 2147483647, 1, &ok);

    if(!ok)
        return;    

    savedImage.setImage(loadedImage.getImage());
    savedImage.adjustContrast(delta);

    displayImage(processedImageLabel, savedImage);
}

void applyNegative(QWidget *parent, QLabel *processedImageLabel, Image &savedImage){
    savedImage.negative();
    
    displayImage(processedImageLabel, savedImage);
}

// change formatting
void applyInvertVertical(QLabel *processedImageLabel, Image &loadedImage, Image &savedImage){
    savedImage.invertVertical();

    displayImage(processedImageLabel, savedImage);
}

void applyInvertHorizontal(QLabel *processedImageLabel, Image &loadedImage, Image &savedImage){
    savedImage.invertHorizontal();
    
    displayImage(processedImageLabel, savedImage);
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
    
    displayImage(processedImageLabel, savedImage);
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

    displayImage(processedImageLabel, savedImage);
}

void applyZoomIn(QWidget *parent, QLabel *processedImageLabel, Image &savedImage){
    savedImage.zoomIn();
    
    displayImage(processedImageLabel, savedImage);
}

// auxiliar
void displayImage(QLabel *imageLabel, Image &displayedImage){
    QImage imgQ(displayedImage.getImage().data, displayedImage.getImage().cols, displayedImage.getImage().rows, displayedImage.getImage().step, QImage::Format_RGB888);
    QPixmap image = QPixmap::fromImage(imgQ.rgbSwapped());

    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setPixmap(image.scaled(imageLabel->size(), Qt::KeepAspectRatio));
    // imageLabel->setPixmap(image);
}