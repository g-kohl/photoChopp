#pragma once

#include <iostream>
#include <opencv2/highgui.hpp>
#include <QVector>

#include "/usr/local/include/opencv4/opencv2/opencv.hpp"

class Image {
public:
	// create/destroy object
	Image(cv::Mat image, std::string &name, int height, int width);
	Image(Image &copy);
	//~Image();

	// attributes
	void setImage(cv::Mat image);
	cv::Mat getImage();
	void setName(std::string &name);
	std::string getName();
	void setSize(int height, int width);
	std::pair<int, int> getSize();

	// files
	std::string getImagePath();
	cv::Mat loadImage();
	void saveImage();
	void showImage();

	// options
	QVector<int> histogram();

	// basic processig
    void RGBtoGrayscale();
    void quantization(int shades);
	void adjustBrightness(int delta);
	void adjustContrast(int delta);
	void negative();

	// change formatting
	void invertVertical();
    void invertHorizontal();
	void rotate(bool clockwise);
	void zoomOut(int sy, int sx);
	void zoomIn();

private:
	cv::Mat image_;
	std::string name_;
	std::pair<int, int> size_;
};