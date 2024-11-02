#include "image.hpp"
#include "button.hpp"

using namespace std;
using namespace cv;

int main(int argc, char *argv[]){
    // VARIABLES:
    Mat defaultMat;
    string defaultName = "name";
    Image loadedImage(defaultMat, defaultName, 1, 1);
    Image savedImage(defaultMat, defaultName, 1, 1);

    // MAIN WINDOW:
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle("Photo Chopp");

    QLabel *loadedImageLabel = new QLabel("");
    QLabel *processedImageLabel = new QLabel("");
    loadedImageLabel->setFixedSize(400, 400);
    processedImageLabel->setFixedSize(400, 400);
    loadedImageLabel->setStyleSheet("border: 1px solid black;");
    processedImageLabel->setStyleSheet("border: 1px solid black;");

    // BUTTONS: files, options, basic processing, change formatting
    // files
    QPushButton *loadButton = new QPushButton("Load Image");
    QObject::connect(loadButton, &QPushButton::clicked, [&]() {
       loadImage(loadedImageLabel, processedImageLabel, loadedImage, savedImage);
    });

    QPushButton *saveButton = new QPushButton("Save Image");
    QObject::connect(saveButton, &QPushButton::clicked, [&]() {
        saveImage(&window, processedImageLabel, savedImage);
    });

    // options
    QPushButton *restoreButton = new QPushButton("Restore Image");
    QObject::connect(restoreButton, &QPushButton::clicked, [&]() {
        restoreImage(processedImageLabel, loadedImage, savedImage);
    });

    QPushButton *histogramModeButton = new QPushButton("Histogram Mode");
    QObject::connect(histogramModeButton, &QPushButton::clicked, [&]() {
        
    });

    // basic processing
    QPushButton *RGBtoGrayscaleButton = new QPushButton("RGB to Grayscale");
    QObject::connect(RGBtoGrayscaleButton, &QPushButton::clicked, [&]() {
        applyRGBtoGrayscale(processedImageLabel, savedImage);
    });

    QPushButton *quantizationButton = new QPushButton("Quantization");
    QObject::connect(quantizationButton, &QPushButton::clicked, [&]() {
        applyQuantization(&window, processedImageLabel, loadedImage, savedImage);
    });

    QPushButton *adjustBrightnessButton = new QPushButton("Adjust Brightness");
    QObject::connect(adjustBrightnessButton, &QPushButton::clicked, [&]() {
        applyAdjustBrightness(&window, processedImageLabel, loadedImage, savedImage);
    });

    QPushButton *adjustContrastButton = new QPushButton("Adjust Contrast");
    QObject::connect(adjustContrastButton, &QPushButton::clicked, [&]() {
        applyAdjustContrast(&window, processedImageLabel, loadedImage, savedImage);
    });

    QPushButton *negativeButton = new QPushButton("Negative");
    QObject::connect(negativeButton, &QPushButton::clicked, [&]() {
        applyNegative(&window, processedImageLabel, savedImage);
    });

    // change formatting
    QPushButton *invertVerticalButton = new QPushButton("Invert Vertical");
    QObject::connect(invertVerticalButton, &QPushButton::clicked, [&]() {
        applyInvertVertical(processedImageLabel, loadedImage, savedImage);
    });

    QPushButton *invertHorizontalButton = new QPushButton("Invert Horizontal");
    QObject::connect(invertHorizontalButton, &QPushButton::clicked, [&]() {
        applyInvertHorizontal(processedImageLabel, loadedImage, savedImage);
    });

    QPushButton *rotateButton = new QPushButton("Rotate");
    QObject::connect(rotateButton, &QPushButton::clicked, [&]() {
        applyRotate(&window, processedImageLabel, savedImage);
    });

    QPushButton *zoomOutButton = new QPushButton("Zoom Out");
    QObject::connect(zoomOutButton, &QPushButton::clicked, [&]() {
        applyZoomOut(&window, processedImageLabel, loadedImage, savedImage);
    });

    QPushButton *zoomInButton = new QPushButton("Zoom In");
    QObject::connect(zoomInButton, &QPushButton::clicked, [&]() {
        applyZoomIn(&window, processedImageLabel, savedImage);
    });

    // LAYOUTS: images, files, options, basic processing, change formatting
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *imageLayout = new QHBoxLayout;
    QHBoxLayout *fileLayout = new QHBoxLayout;
    QHBoxLayout *optionsLayout = new QHBoxLayout;
    QHBoxLayout *basicProcessingLayout = new QHBoxLayout;
    QHBoxLayout *changeFormattingLayout = new QHBoxLayout;

    // images
    imageLayout->addWidget(loadedImageLabel);
    imageLayout->addWidget(processedImageLabel);

    // files
    fileLayout->addWidget(loadButton);
    fileLayout->addWidget(saveButton);

    // options
    optionsLayout->addWidget(restoreButton);
    optionsLayout->addWidget(histogramModeButton);

    // basic processing
    basicProcessingLayout->addWidget(RGBtoGrayscaleButton);
    basicProcessingLayout->addWidget(quantizationButton);
    basicProcessingLayout->addWidget(adjustBrightnessButton);
    basicProcessingLayout->addWidget(adjustContrastButton);
    basicProcessingLayout->addWidget(negativeButton);

    // change formatting
    changeFormattingLayout->addWidget(invertVerticalButton);
    changeFormattingLayout->addWidget(invertHorizontalButton);
    changeFormattingLayout->addWidget(rotateButton);
    changeFormattingLayout->addWidget(zoomOutButton);
    changeFormattingLayout->addWidget(zoomInButton);

    // GROUPS:
    QGroupBox *basicProcessingGroup = new QGroupBox("Basic Processing");
    QGroupBox *changeFormattingGroup = new QGroupBox("Change Formatting");

    basicProcessingGroup->setLayout(basicProcessingLayout);
    changeFormattingGroup->setLayout(changeFormattingLayout);

    // DISPLAY WINDOW:
    mainLayout->addLayout(imageLayout);
    mainLayout->addLayout(fileLayout);
    mainLayout->addLayout(optionsLayout);
    mainLayout->addWidget(basicProcessingGroup);
    mainLayout->addWidget(changeFormattingGroup);

    window.setLayout(mainLayout);
    window.show();

    return app.exec();
}


/*
https://docs.opencv.org/4.x/d8/dbc/tutorial_histogram_calculation.html
https://doc.qt.io/qt-5/qtcharts-barchart-example.html
*/