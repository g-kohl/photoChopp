#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <stdio.h>
#include "image.hpp"

using namespace std;
using namespace cv;

void loadImage(QLabel *loadedImage, QLabel *processedImage, Image &loadImg, Image &saveImg){
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Load image", "", "Images (*.png *.jpg *.bmp)");
    string imageName = fileName.toStdString();
    Mat matImage = imread(imageName, IMREAD_COLOR);

    if(matImage.empty()){
        cout << "Error: empty image";
        return;
    }

    loadImg.setImage(matImage);
    loadImg.setName(imageName);
    loadImg.setSize(matImage.rows, matImage.cols);

    saveImg.setImage(matImage);
    saveImg.setName(imageName);
    saveImg.setSize(matImage.rows, matImage.cols);

    if(!fileName.isEmpty()) {
        QPixmap image(fileName);
        loadedImage->setPixmap(image.scaled(loadedImage->size(), Qt::KeepAspectRatio));
        processedImage->setPixmap(image.scaled(loadedImage->size(), Qt::KeepAspectRatio));
    }
}

void flipVertical(QLabel *imageLabel, Image &loadImg, Image &saveImg){
    saveImg.invertImageVertical();

    QImage imgQ(saveImg.getImage().data, saveImg.getImage().cols, saveImg.getImage().rows, saveImg.getImage().step, QImage::Format_RGB888);
    QPixmap image = QPixmap::fromImage(imgQ.rgbSwapped());

    imageLabel->setPixmap(image.scaled(imageLabel->size(), Qt::KeepAspectRatio));
}

void flipHorizontal(QLabel *imageLabel, Image &loadImg, Image &saveImg){
    saveImg.invertImageHorizontal();

    QImage imgQ(saveImg.getImage().data, saveImg.getImage().cols, saveImg.getImage().rows, saveImg.getImage().step, QImage::Format_RGB888);
    QPixmap image = QPixmap::fromImage(imgQ.rgbSwapped());

    imageLabel->setPixmap(image.scaled(imageLabel->size(), Qt::KeepAspectRatio));
}

void applyGrayscale(QLabel *imageLabel, Image &loadImg, Image &saveImg){
    Mat grayMat = saveImg.RGBtoGrayscale().clone();
    saveImg.setImage(grayMat);

    QImage imgQ(grayMat.data, grayMat.cols, grayMat.rows, grayMat.step, QImage::Format_Grayscale8);
    QPixmap image = QPixmap::fromImage(imgQ);

    imageLabel->setPixmap(image.scaled(imageLabel->size(), Qt::KeepAspectRatio));
}

void applyQuantization(QWidget *parent, QLabel *imageLabel, Image &loadImg, Image &saveImg){
    Image quantized(loadImg);
    String defaultName = "default";
    bool ok;
    int shades = QInputDialog::getInt(parent, "Quantization", "Enter the number of shades:", 0, -2147483647, 2147483647, 1, &ok);
    Mat grayMat = quantized.RGBtoGrayscale();

    Image gray(grayMat, defaultName, grayMat.rows, grayMat.cols);

    Mat quantizedMat = gray.quantization(shades).clone();
    saveImg.setImage(quantizedMat);

    QImage imgQ(quantizedMat.data, quantizedMat.cols, quantizedMat.rows, quantizedMat.step, QImage::Format_Grayscale8);
    QPixmap image = QPixmap::fromImage(imgQ);

    imageLabel->setPixmap(image.scaled(imageLabel->size(), Qt::KeepAspectRatio));
}

void saveImage(QWidget *parent, QLabel *imageLabel, Image &saveImg){
    bool ok;
    QString text = QInputDialog::getText(parent, "Save Image", "Enter the name of the file (with extension)", QLineEdit::Normal, "", &ok);
    string fileName = text.toStdString();

    saveImg.setName(fileName);
    saveImg.saveImage();
}

int main(int argc, char *argv[]){
    Mat emptyMat;
    string defaultName = "name";
    Image loadedImage(emptyMat, defaultName, 1, 1);
    Image savedImage(emptyMat, defaultName, 1, 1);

    QApplication app(argc, argv);
    QWidget window;

    window.setWindowTitle("Photo Chopp");

    QLabel *loadedImageLabel = new QLabel("");
    QLabel *processedImageLabel = new QLabel("");

    loadedImageLabel->setFixedSize(400, 400);
    processedImageLabel->setFixedSize(400, 400);
    loadedImageLabel->setStyleSheet("border: 1px solid black;");
    processedImageLabel->setStyleSheet("border: 1px solid black;");

    QPushButton *loadButton = new QPushButton("Load Image");
    QPushButton *saveButton = new QPushButton("Save Image");
    QPushButton *flipVerticalButton = new QPushButton("Flip Vertical");
    QPushButton *flipHorizontalButton = new QPushButton("Flip Horizontal");
    QPushButton *grayscaleButton = new QPushButton("Grayscale");
    QPushButton *quantizationButton = new QPushButton("Quantization");

    QObject::connect(loadButton, &QPushButton::clicked, [&]() {
       loadImage(loadedImageLabel, processedImageLabel, loadedImage, savedImage);
    });

    QObject::connect(saveButton, &QPushButton::clicked, [&]() {
        saveImage(&window, processedImageLabel, savedImage);
    });

    QObject::connect(flipVerticalButton, &QPushButton::clicked, [&]() {
        flipVertical(processedImageLabel, loadedImage, savedImage);
    });

    QObject::connect(flipHorizontalButton, &QPushButton::clicked, [&]() {
        flipHorizontal(processedImageLabel, loadedImage, savedImage);
    });

    QObject::connect(grayscaleButton, &QPushButton::clicked, [&]() {
        applyGrayscale(processedImageLabel, loadedImage, savedImage);
    });

    QObject::connect(quantizationButton, &QPushButton::clicked, [&]() {
        applyQuantization(&window, processedImageLabel, loadedImage, savedImage);
    });

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *imageLayout = new QHBoxLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;

    imageLayout->addWidget(loadedImageLabel);
    imageLayout->addWidget(processedImageLabel);

    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(flipVerticalButton);
    buttonLayout->addWidget(flipHorizontalButton);
    buttonLayout->addWidget(grayscaleButton);
    buttonLayout->addWidget(quantizationButton);

    mainLayout->addLayout(imageLayout);
    mainLayout->addLayout(buttonLayout);

    window.setLayout(mainLayout);

    window.show();

    return app.exec();
}
