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

// operate on images
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

// Mat Image::RGBtoGrayscale(){
//     int height = size_.first, width = size_.second;
//     Mat grayscale(height, width, CV_8UC1, Scalar(0));
//     uchar l;

//     for(int i=0; i<height; i++){
//         for(int j=0; j<width; j++){
//             Vec3b pixel = image_.at<Vec3b>(i, j);
//             l = 0.114*pixel[0] + 0.587*pixel[1] + 0.299*pixel[2];
//             grayscale.at<uchar>(i, j) = l;
//         }
//     }

//     return grayscale;
// }

void Image::RGBtoGrayscale(){
    int height = size_.first, width = size_.second, luminance;
    Vec3b pixel;
    Mat grayscale(height, width, CV_8UC3);

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            pixel = image_.at<Vec3b>(i, j);
            luminance = 0.114 * pixel[0] + 0.587 * pixel[1] + 0.299 * pixel[2];

            grayscale.at<Vec3b>(i, j) = Vec3b(luminance, luminance, luminance);
        }
    }

    image_ = grayscale.clone();
}

void Image::quantization(int shades){
    int height = size_.first, width = size_.second, intPixel;
    float shadesList[shades+1], darkestPixel = 255, brightestPixel = 0, shadesQuantity, binSize, floatPixel;
    Vec3b pixel;
    Mat quantized(height, width, CV_8UC3);

    for(int i=0; i<height; i++){
        for(int j=0; j<height; j++){
            pixel = image_.at<Vec3b>(i, j);
            intPixel = int(pixel[1]);

            if(intPixel < darkestPixel)
                darkestPixel = intPixel;
            if(intPixel > brightestPixel)
                brightestPixel = intPixel;
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
                pixel = image_.at<Vec3b>(i, j);
                floatPixel = float(pixel[1]);

                while(floatPixel > shadesList[k]){
                    k++;
                }

                intPixel = round((shadesList[k]+shadesList[k-1])/2);
                quantized.at<Vec3b>(i, j) = Vec3b(intPixel, intPixel, intPixel);
            }
        }

        image_ = quantized.clone();
    }
}