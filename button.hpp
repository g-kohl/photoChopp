#pragma once

#include <QApplication>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

#include "image.hpp"

void loadImage(QLabel *loadedImage, QLabel *processedImage, Image &loadImg, Image &saveImg);

void saveImage(QWidget *parent, QLabel *imageLabel, Image &saveImg);

void flipVertical(QLabel *imageLabel, Image &loadImg, Image &saveImg);

void flipHorizontal(QLabel *imageLabel, Image &loadImg, Image &saveImg);

void applyGrayscale(QLabel *imageLabel, Image &loadImg, Image &saveImg);

void applyQuantization(QWidget *parent, QLabel *imageLabel, Image &loadImg, Image &saveImg);