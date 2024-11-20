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

    // INPUTS:
    QVector<QLineEdit*> convolutionInputs;

    for(int i=0; i<INPUTS_NUM; i++){
        QLineEdit *convolutionInput = new QLineEdit;

        convolutionInputs.append(convolutionInput);
    }

    // BUTTONS: files, options, basic processing, change formatting, convolution, histogram
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
        applyNegative(processedImageLabel, savedImage);
    });

    // change formatting
    QPushButton *invertVerticalButton = new QPushButton("Invert Vertical");
    QObject::connect(invertVerticalButton, &QPushButton::clicked, [&]() {
        applyInvertVertical(processedImageLabel, savedImage);
    });

    QPushButton *invertHorizontalButton = new QPushButton("Invert Horizontal");
    QObject::connect(invertHorizontalButton, &QPushButton::clicked, [&]() {
        applyInvertHorizontal(processedImageLabel, savedImage);
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

    // convolution
    QPushButton *gaussianButton = new QPushButton("Gaussian");
    QObject::connect(gaussianButton, &QPushButton::clicked, [&]() {
        vector<float> kernel = {0.0625, 0.125, 0.0625, 0.125, 0.25, 0.125, 0.0625, 0.125, 0.0625};

        applyConvolution(processedImageLabel, savedImage, kernel, false, false);
    });

    QPushButton *laplacianButton = new QPushButton("Laplacian");
    QObject::connect(laplacianButton, &QPushButton::clicked, [&]() {
        vector<float> kernel = {0, -1, 0, -1, 4, -1, 0, -1, 0};

        applyConvolution(processedImageLabel, savedImage, kernel, true, false);
    });

    QPushButton *highPassButton = new QPushButton("High Pass");
    QObject::connect(highPassButton, &QPushButton::clicked, [&]() {
        vector<float> kernel = {-1, -1, -1, -1, 8, -1, -1, -1, -1};

        applyConvolution(processedImageLabel, savedImage, kernel, true, false);
    });

    QPushButton *prewittHxButton = new QPushButton("Prewitt Hx");
    QObject::connect(prewittHxButton, &QPushButton::clicked, [&]() {
        vector<float> kernel = {-1, 0, 1, -1, 0, 1, -1, 0, 1};

        applyConvolution(processedImageLabel, savedImage, kernel, true, true);
    });

    QPushButton *prewittHyButton = new QPushButton("Prewitt Hy");
    QObject::connect(prewittHyButton, &QPushButton::clicked, [&]() {
        vector<float> kernel = {-1, -1, -1, 0, 0, 0, 1, 1, 1};

        applyConvolution(processedImageLabel, savedImage, kernel, true, true);
    });

    QPushButton *sobelHxButton = new QPushButton("Sobel Hx");
    QObject::connect(sobelHxButton, &QPushButton::clicked, [&]() {
        vector<float> kernel = {-1, 0, 1, -2, 0, 2, -1, 0, 1};

        applyConvolution(processedImageLabel, savedImage, kernel, true, true);
    });

    QPushButton *sobelHyButton = new QPushButton("Sobel Hy");
    QObject::connect(sobelHyButton, &QPushButton::clicked, [&]() {
    vector<float> kernel = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

        applyConvolution(processedImageLabel, savedImage, kernel, true, true);
    });

    QPushButton *customConvolutionButton = new QPushButton("Custom Convolution");
    QObject::connect(customConvolutionButton, &QPushButton::clicked, [&]() {
        vector<float> kernel = readInputs(convolutionInputs);

        applyConvolution(processedImageLabel, savedImage, kernel, true, false);
    });

    // histogram
    QPushButton *showHistogramButton = new QPushButton("Show Histogram");
    QObject::connect(showHistogramButton, &QPushButton::clicked, [&]() {
        showHistogram(processedImageLabel, savedImage);
    });

    QPushButton *equalizeHistogramButton = new QPushButton("Equalize Histogram");
    QObject::connect(equalizeHistogramButton, &QPushButton::clicked, [&]() {
        applyEqualization(processedImageLabel, savedImage);
    });

    QPushButton *histogramMatchingButton = new QPushButton("Histogram Matching");
    QObject::connect(histogramMatchingButton, &QPushButton::clicked, [&]() {
        loadNextImage(loadedImageLabel, processedImageLabel, loadedImage, savedImage);
        applyHistogramMatching(processedImageLabel, loadedImage, savedImage);
    });

    // LAYOUTS: images, files, options, basic processing, change formatting, convolution, histogram
    // images
    QHBoxLayout *imageLayout = new QHBoxLayout;

    imageLayout->addWidget(loadedImageLabel);
    imageLayout->addWidget(processedImageLabel);

    // files
    QHBoxLayout *fileLayout = new QHBoxLayout;

    fileLayout->addWidget(loadButton);
    fileLayout->addWidget(saveButton);

    // options
    QHBoxLayout *optionsLayout = new QHBoxLayout;

    optionsLayout->addWidget(restoreButton);

    // basic processing
    QHBoxLayout *basicProcessingLayout = new QHBoxLayout;

    basicProcessingLayout->addWidget(RGBtoGrayscaleButton);
    basicProcessingLayout->addWidget(quantizationButton);
    basicProcessingLayout->addWidget(adjustBrightnessButton);
    basicProcessingLayout->addWidget(adjustContrastButton);
    basicProcessingLayout->addWidget(negativeButton);

    // change formatting
    QHBoxLayout *changeFormattingLayout = new QHBoxLayout;

    changeFormattingLayout->addWidget(invertVerticalButton);
    changeFormattingLayout->addWidget(invertHorizontalButton);
    changeFormattingLayout->addWidget(rotateButton);
    changeFormattingLayout->addWidget(zoomOutButton);
    changeFormattingLayout->addWidget(zoomInButton);

    // convolution
    QVBoxLayout *mainConvolutionLayout = new QVBoxLayout;
    QHBoxLayout *convolutionLayout = new QHBoxLayout;
    QHBoxLayout *customConvolutionLayout = new QHBoxLayout;
    QVBoxLayout *customConvolutionLines = new QVBoxLayout;
    QHBoxLayout *customConvolutionLine1 = new QHBoxLayout;
    QHBoxLayout *customConvolutionLine2 = new QHBoxLayout;
    QHBoxLayout *customConvolutionLine3 = new QHBoxLayout;

    mainConvolutionLayout->addLayout(convolutionLayout);
    mainConvolutionLayout->addLayout(customConvolutionLayout);

    convolutionLayout->addWidget(gaussianButton);
    convolutionLayout->addWidget(laplacianButton);
    convolutionLayout->addWidget(highPassButton);
    convolutionLayout->addWidget(prewittHxButton);
    convolutionLayout->addWidget(prewittHyButton);
    convolutionLayout->addWidget(sobelHxButton);
    convolutionLayout->addWidget(sobelHyButton);

    customConvolutionLayout->addLayout(customConvolutionLines);
    customConvolutionLayout->addWidget(customConvolutionButton);

    customConvolutionLines->addLayout(customConvolutionLine1);
    customConvolutionLines->addLayout(customConvolutionLine2);
    customConvolutionLines->addLayout(customConvolutionLine3);

    customConvolutionLine1->addWidget(convolutionInputs[0]);
    customConvolutionLine1->addWidget(convolutionInputs[1]);
    customConvolutionLine1->addWidget(convolutionInputs[2]);
    customConvolutionLine2->addWidget(convolutionInputs[3]);
    customConvolutionLine2->addWidget(convolutionInputs[4]);
    customConvolutionLine2->addWidget(convolutionInputs[5]);
    customConvolutionLine3->addWidget(convolutionInputs[6]);
    customConvolutionLine3->addWidget(convolutionInputs[7]);
    customConvolutionLine3->addWidget(convolutionInputs[8]);

    // histogram
    QHBoxLayout *histogramLayout = new QHBoxLayout;

    histogramLayout->addWidget(showHistogramButton);
    histogramLayout->addWidget(equalizeHistogramButton);
    histogramLayout->addWidget(histogramMatchingButton);

    // GROUPS:
    QGroupBox *basicProcessingGroup = new QGroupBox("Basic Processing");
    QGroupBox *changeFormattingGroup = new QGroupBox("Change Formatting");
    QGroupBox *convolutionGroup = new QGroupBox("Convolution");
    QGroupBox *histogramGroup = new QGroupBox("Histogram");

    basicProcessingGroup->setLayout(basicProcessingLayout);
    changeFormattingGroup->setLayout(changeFormattingLayout);
    convolutionGroup->setLayout(mainConvolutionLayout);
    histogramGroup->setLayout(histogramLayout);

    // DISPLAY WINDOW:
    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addLayout(imageLayout);
    mainLayout->addLayout(fileLayout);
    mainLayout->addLayout(optionsLayout);
    mainLayout->addWidget(basicProcessingGroup);
    mainLayout->addWidget(changeFormattingGroup);
    mainLayout->addWidget(convolutionGroup);
    mainLayout->addWidget(histogramGroup);

    window.setLayout(mainLayout);
    window.show();

    return app.exec();
}


/*
https://docs.opencv.org/4.x/d8/dbc/tutorial_histogram_calculation.html
https://doc.qt.io/qt-5/qtcharts-barchart-example.html
*/