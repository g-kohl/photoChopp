#pragma once

#include <algorithm>
#include <QApplication>
#include <QBarSeries>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QEvent>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QValueAxis>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "image.hpp"

#define INPUTS_NUM 9

// files
void loadImage(QLabel *loadedImageLabel, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage);

void saveImage(QWidget *parent, QLabel *imageLabel, Image &savedImage);

// options
void restoreImage(QLabel *processedImageLabel, Image &loadedImage, Image &savedImage);

void loadNextImage(QLabel *loadedImageLabel, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage);

// basic processing
void applyRGBtoGrayscale(QLabel *processedImageLabel, Image &savedImage);

void applyQuantization(QWidget *parent, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage);

void applyAdjustBrightness(QWidget *parent, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage);

void applyAdjustContrast(QWidget *parent, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage);

void applyNegative(QLabel *processedImageLabel, Image &savedImage);

// change formatting
void applyInvertVertical(QLabel *processedImageLabel, Image &savedImage);

void applyInvertHorizontal(QLabel *processedImageLabel, Image &savedImage);

void applyRotate(QWidget *parent, QLabel *processedImageLabel, Image &savedImage);

void applyZoomOut(QWidget *parent, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage);

void applyZoomIn(QWidget *parent, QLabel *processedImageLabel, Image &savedImage);

// convolution
void applyConvolution(QLabel *processedImageLabel, Image &savedImage, std::vector<float> kernel, bool grayscale, bool clamping);

// histogram
void showHistogram(QLabel *processedImageLabel, Image &savedImage);

void applyEqualization(QLabel *processedImageLabel, Image &savedImage);

void applyHistogramMatching(QLabel *processedImageLabel, Image &loadedImage, Image &savedImage);

// auxiliary
void displayImage(QLabel *imageLabel, Image &displayedImage, bool scale);

std::vector<float> readInputs(QVector<QLineEdit*> convolutionInputs);