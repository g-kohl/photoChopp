#include "image.hpp"

using namespace std;
using namespace cv;

// create/destroy object
Image::Image(Mat image, string& name, int height, int width) : name_(name), size_{height, width} {}

Image::Image(Image& copy) : image_(copy.image_), name_(copy.name_), size_(copy.size_) {};

// operate on attributes
void Image::setImage(Mat image) {
	image_ = image;
}

Mat Image::getImage() {
	return image_;
}

void Image::setName(string &name) {
	name_ = name;
}

string Image::getName(){
	return name_;
}

void Image::setSize(int height, int width) {
	size_.first = height;
	size_.second = width;
}

pair<int, int> Image::getSize() {
	return size_;
}

// operate on files
string Image::getImagePath() {
	return samples::findFile(name_);
}

Mat Image::loadImage() {
	return imread(getImagePath(), IMREAD_COLOR);
}

void Image::saveImage(Mat& image, string& name) {
	imwrite(name, image);
}

void Image::showImage(Mat& image) {
	imshow("Display window", image);
}

// operate on images
//void Image::changeBrightness(int delta) {
//	for (int i = 0; i < size_.first; i++) {
//		for (int j = 0; j < size_.second; j++) {
//			image_.at<uchar>(i, j);
//		}
//	}
//}

void Image::invertImageVertical() {

}