#include "image.hpp"
#include "button.hpp"

using namespace std;
using namespace cv;

int main(int argc, char *argv[]){
    Mat defaultMat;
    string defaultName = "name";
    Image loadedImage(defaultMat, defaultName, 1, 1);
    Image savedImage(defaultMat, defaultName, 1, 1);

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
    QObject::connect(loadButton, &QPushButton::clicked, [&]() {
       loadImage(loadedImageLabel, processedImageLabel, loadedImage, savedImage);
    });

    QPushButton *saveButton = new QPushButton("Save Image");
    QObject::connect(saveButton, &QPushButton::clicked, [&]() {
        saveImage(&window, processedImageLabel, savedImage);
    });

    QPushButton *flipVerticalButton = new QPushButton("Flip Vertical");
    QObject::connect(flipVerticalButton, &QPushButton::clicked, [&]() {
        flipVertical(processedImageLabel, loadedImage, savedImage);
    });

    QPushButton *flipHorizontalButton = new QPushButton("Flip Horizontal");
    QObject::connect(flipHorizontalButton, &QPushButton::clicked, [&]() {
        flipHorizontal(processedImageLabel, loadedImage, savedImage);
    });

    QPushButton *grayscaleButton = new QPushButton("Grayscale");
    QObject::connect(grayscaleButton, &QPushButton::clicked, [&]() {
        applyGrayscale(processedImageLabel, loadedImage, savedImage);
    });

    QPushButton *quantizationButton = new QPushButton("Quantization");
    QObject::connect(quantizationButton, &QPushButton::clicked, [&]() {
        applyQuantization(&window, processedImageLabel, loadedImage, savedImage);
    });

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *imageLayout = new QHBoxLayout;
    QHBoxLayout *fileLayout = new QHBoxLayout;
    QHBoxLayout *processingLayout = new QHBoxLayout;

    imageLayout->addWidget(loadedImageLabel);
    imageLayout->addWidget(processedImageLabel);

    fileLayout->addWidget(loadButton);
    fileLayout->addWidget(saveButton);
    
    processingLayout->addWidget(flipVerticalButton);
    processingLayout->addWidget(flipHorizontalButton);
    processingLayout->addWidget(grayscaleButton);
    processingLayout->addWidget(quantizationButton);

    mainLayout->addLayout(imageLayout);
    mainLayout->addLayout(fileLayout);
    mainLayout->addLayout(processingLayout);

    window.setLayout(mainLayout);
    window.show();

    return app.exec();
}
