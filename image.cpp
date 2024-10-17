#include "image.hpp"
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

// create/destroy object
Image::Image(Mat image, string &name, int height, int width) : image_(image.clone()), name_(name), size_{height, width}{};

Image::Image(Image &copy) : image_(copy.image_), name_(copy.name_), size_(copy.size_){};

// operate on attributes
void Image::setImage(Mat image){
	image_ = image;
}

Mat Image::getImage(){
	return image_;
}

void Image::setName(string &name){
	name_ = name;
}

string Image::getName(){
	return name_;
}

void Image::setSize(int height, int width){
	size_.first = height;
	size_.second = width;
}

pair<int, int> Image::getSize(){
	return size_;
}

// operate on files
string Image::getImagePath(){
	return samples::findFile(name_);
}

Mat Image::loadImage(){
	return imread(getImagePath(), IMREAD_COLOR);
}

void Image::saveImage(){
	imwrite(name_, image_);
}

void Image::showImage(){
	imshow("Display window", image_);
}

void Image::invertImageVertical(){
    int height = size_.first;
    Mat temp_row;

    for(int i=0; i<height/2; i++){
        image_.row(i).copyTo(temp_row);
        image_.row(height-1-i).copyTo(image_.row(i));
        temp_row.copyTo(image_.row(height-1-i));
    }
}

void Image::invertImageHorizontal(){
    int width = size_.second;
    Mat temp_col;

    for(int i=0; i<width/2; i++){
        image_.col(i).copyTo(temp_col);
        image_.col(width-1-i).copyTo(image_.col(i));
        temp_col.copyTo(image_.col(width-1-i));
    }
}

Mat Image::RGBtoGrayscale(){
    int height = size_.first, width = size_.second;
    Mat grayscale(height, width, CV_8UC1, Scalar(0));
    uchar l;

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            Vec3b pixel = image_.at<Vec3b>(i, j);
            l = 0.114*pixel[0] + 0.587*pixel[1] + 0.299*pixel[2];
            grayscale.at<uchar>(i, j) = l;
        }
    }

    return grayscale;
}

Mat Image::quantization(int shades){
    int height = size_.first, width = size_.second, intP;
    float shadesList[shades+1], darkestPixel = 255, brightestPixel = 0, shadesQuantity, binSize, floatP;
    Mat quantized(height, width, CV_8UC1, Scalar(0));
    uchar p;

    for(int i=0; i<height; i++){
        for(int j=0; j<height; j++){
            p = image_.at<uchar>(i, j);
            intP = int(p);

            if(intP < darkestPixel)
                darkestPixel = intP;
            if(intP > brightestPixel)
                brightestPixel = intP;
        }
    }

    shadesQuantity = brightestPixel - darkestPixel + 1;

    if(shades < shadesQuantity){
        binSize = shadesQuantity / shades;

        for(int i=0; i<=shades; i++){
            shadesList[i] = (darkestPixel-0.5) + i * binSize;
        }

        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                int k = 0;
                p = image_.at<uchar>(i,j);
                floatP = float(p);

                while(floatP > shadesList[k]){
                    k++;
                }

                // cout << shadesList[k] << " " << shadesList[k-1] << " " << round((shadesList[k]+shadesList[k-1])/2) << endl;
                quantized.at<uchar>(i, j) = uchar(round((shadesList[k]+shadesList[k-1])/2));
            }
        }

        return quantized;
    }

    return image_;
}