#pragma once

#include <algorithm>
#include <QApplication>
#include <QEvent>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "image.hpp"

// files
void loadImage(QLabel *loadedImageLabel, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage);

void saveImage(QWidget *parent, QLabel *imageLabel, Image &savedImage);

// options
void restoreImage(QLabel *processedImageLabel, Image &loadedImage, Image &savedImage);
void generateHistogram(QWidget *parent, QLabel *imageLabel, Image &savedImage);

// basic processing
void applyRGBtoGrayscale(QLabel *processedImageLabel, Image &savedImage);

void applyQuantization(QWidget *parent, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage);

void applyAdjustBrightness(QWidget *parent, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage);

void applyAdjustContrast(QWidget *parent, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage);

void applyNegative(QWidget *parent, QLabel *processedImageLabel, Image &savedImage);

// change formatting
void applyInvertVertical(QLabel *processedImageLabel, Image &loadedImage, Image &savedImage);

void applyInvertHorizontal(QLabel *processedImageLabel, Image &loadedImage, Image &savedImage);

void applyRotate(QWidget *parent, QLabel *processedImageLabel, Image &savedImage);

void applyZoomOut(QWidget *parent, QLabel *processedImageLabel, Image &loadedImage, Image &savedImage);

void applyZoomIn(QWidget *parent, QLabel *processedImageLabel, Image &savedImage);

// auxiliar
void displayImage(QLabel *imageLabel, Image &displayedImage);