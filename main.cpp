#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <stdio.h>
#include "image.hpp"

using namespace std;
using namespace cv;

Mat loadImage(QLabel *imageLabel){
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Select image", "", "Images (*.png *.jpg *.bmp)");
    string imageName = fileName.toStdString();
    Mat matImage = imread(imageName, IMREAD_COLOR);

    if(matImage.empty()){
        cout << "Error: empty image";
        return;
    }

    if (!fileName.isEmpty()) {
        QPixmap image(fileName);
        imageLabel->setPixmap(image.scaled(imageLabel->size(), Qt::KeepAspectRatio));

        // if (!image.isNull()) {
        //     // Ajusta o pixmap ao tamanho do label e o define no QLabel
        //     imageLabel->setPixmap(image.scaled(imageLabel->size(), Qt::KeepAspectRatio));
        // } else {
        //     // Exibe uma mensagem de erro se a imagem não puder ser carregada
        //     QMessageBox::warning(nullptr, "Erro", "Não foi possível carregar a imagem.");
        // }
    }

    return matImage;
}

int main(int argc, char *argv[]){
    Mat emptyMat;
    string defaultName = "name";

    QApplication app(argc, argv);
    QWidget window;

    window.setWindowTitle("Photo Chopp");

    QLabel *loadedImageLabel = new QLabel("");
    QLabel *processedImageLabel = new QLabel("");

    loadedImageLabel->setFixedSize(400, 400);
    processedImageLabel->setFixedSize(400, 400);
    loadedImageLabel->setStyleSheet("border: 1px solid black;");
    processedImageLabel->setStyleSheet("border: 1px solid black;");

    QPushButton *loadButton = new QPushButton("Load image");
    QPushButton *processButton = new QPushButton("Process image");

    QObject::connect(loadButton, &QPushButton::clicked, [=]() {
       emptyMat = loadImage(loadedImageLabel).clone();
    });

    QObject::connect(processButton, &QPushButton::clicked, [=]() {
        loadImage(loadedImageLabel);
    });

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *imageLayout = new QHBoxLayout;
    QHBoxLayout *buttonLayout = new QHBoxLayout;

    imageLayout->addWidget(loadedImageLabel);
    imageLayout->addWidget(processedImageLabel);

    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(processButton);

    mainLayout->addLayout(imageLayout);
    mainLayout->addLayout(buttonLayout);

    window.setLayout(mainLayout);

    window.show();

    return app.exec();
}


// int main(){
//     string imgName = "Gramado_72k.jpg";
//     Mat mat = imread(imgName, IMREAD_COLOR);

//     if (mat.empty()) {
//         cout << "Error: empty image";
//         return 0;
//     }

//     Image img(mat, imgName, mat.rows, mat.cols);

//     Mat gray = img.RGBtoGrayscale().clone();
//     imshow("Display window", gray);
//     waitKey(0);
//     imgName = "gray";

//     Image img_gray(gray, imgName, gray.rows, gray.cols);

//     Mat quantized = img_gray.quantization(5);
//     imshow("Display window", quantized);
//     waitKey(0);

// 	return 0;
// }