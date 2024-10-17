#pragma once

#include "/usr/local/include/opencv4/opencv2/opencv.hpp"

// #include <opencv2/core.hpp>
// #include <opencv2/imgcodecs.hpp>
// #include <opencv2/highgui.hpp>
#include <iostream>

class Image {
public:
	// create/destroy object
	Image(cv::Mat image, std::string &name, int height, int width);
	Image(Image &copy);
	//~Image();

	// operate on attributes
	void setImage(cv::Mat image);
	cv::Mat getImage();
	void setName(std::string &name);
	std::string getName();
	void setSize(int height, int width);
	std::pair<int, int> getSize();

	// operate on files
	std::string getImagePath();
	cv::Mat loadImage();
	void saveImage();
	void showImage();

	// operate on images
	void invertImageVertical();
    void invertImageHorizontal();
    cv::Mat RGBtoGrayscale();
    cv::Mat quantization(int shades);

private:
	cv::Mat image_;
	std::string name_;
	std::pair<int, int> size_;
};