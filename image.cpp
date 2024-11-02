#include "image.hpp"

using namespace std;
using namespace cv;

// create/destroy object
Image::Image(Mat image, string &name, int height, int width) : image_(image.clone()), name_(name), size_{height, width}{};

Image::Image(Image &copy) : image_(copy.image_), name_(copy.name_), size_(copy.size_){};

// attributes
void Image::setImage(Mat image){
	image_ = image.clone();
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

// files
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

// options
QVector<int> Image::histogram(){
    int height = size_.first, width = size_.second, shade;
    QVector<int> shades(256, 0);

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            shade = int(image_.at<Vec3b>(i, j)[0]);
            shades[shade]++;
        }
    }

    return shades;
}

// basic processing
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

void Image::adjustBrightness(int delta){
    int height = size_.first, width = size_.second, channel;

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            for(int k=0; k<3; k++){
                channel = int(image_.at<Vec3b>(i, j)[k]) + delta;
                
                if(channel < 0)
                    channel = 0;
                else if(channel > 255)
                    channel = 255;

                image_.at<Vec3b>(i, j)[k] = channel;
            }
        }
    }
}

void Image::adjustContrast(int delta){
    int height = size_.first, width = size_.second, channel;

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            for(int k=0; k<3; k++){
                channel = int(image_.at<Vec3b>(i, j)[k]) * delta;
                
                if(channel < 0)
                    channel = 0;
                else if(channel > 255)
                    channel = 255;

                image_.at<Vec3b>(i, j)[k] = channel;
            }
        }
    }
}

void Image::negative(){
    int height = size_.first, width = size_.second, channel;

    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            for(int k=0; k<3; k++){
                channel = 255 - int(image_.at<Vec3b>(i, j)[k]);
                image_.at<Vec3b>(i, j)[k] = channel;
            }
        }
    }
}

// change formatting
void Image::invertVertical(){
    int height = size_.first;
    Mat temp_row;

    for(int i=0; i<height/2; i++){
        image_.row(i).copyTo(temp_row);
        image_.row(height-1-i).copyTo(image_.row(i));
        temp_row.copyTo(image_.row(height-1-i));
    }
}

void Image::invertHorizontal(){
    int width = size_.second;
    Mat temp_col;

    for(int i=0; i<width/2; i++){
        image_.col(i).copyTo(temp_col);
        image_.col(width-1-i).copyTo(image_.col(i));
        temp_col.copyTo(image_.col(width-1-i));
    }
}

void Image::rotate(bool clockwise){
    int height = size_.first, width = size_.second;
    Mat rotated(width, height, CV_8UC3);

    if(clockwise){
        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                rotated.at<Vec3b>(j, height-i-1) = image_.at<Vec3b>(i, j); 
            }
        }
    }
    else{
        for(int i=0; i<height; i++){
            for(int j=0; j<width; j++){
                rotated.at<Vec3b>(width-j-1, i) = image_.at<Vec3b>(i, j);
            }
        }
    }

    image_ = rotated.clone();
    size_ = {width, height};
}

void Image::zoomOut(int sy, int sx){
    float height = float(size_.first), width = float(size_.second);
    int zoomedHeight = ceil(height/sy), zoomedWidth = ceil(width/sx), averageRed, averageGreen, averageBlue, pixelsCounter;
    Mat zoomed(zoomedHeight, zoomedWidth, CV_8UC3);

    for(int i=0; i<zoomedHeight; i++){
        for(int j=0; j<zoomedWidth; j++){
            averageRed = 0;
            averageGreen = 0;
            averageBlue = 0;
            pixelsCounter = 0;

            for(int k=0; k<sy; k++){
                if((sy*i)+k >= int(height))
                    break;

                for(int l=0; l<sx; l++){
                    if((sx*j)+l >= int(width))
                        break;

                    averageRed += int(image_.at<Vec3b>((sy*i)+k, (sx*j)+l)[2]);
                    averageGreen += int(image_.at<Vec3b>((sy*i)+k, (sx*j)+l)[1]);
                    averageBlue += int(image_.at<Vec3b>((sy*i)+k, (sx*j)+l)[0]);
                    pixelsCounter ++;
                }
            }

            zoomed.at<Vec3b>(i, j)[2] = averageRed / pixelsCounter;
            zoomed.at<Vec3b>(i, j)[1] = averageGreen / pixelsCounter;
            zoomed.at<Vec3b>(i, j)[0] = averageBlue / pixelsCounter;
        }
    }

    image_ = zoomed.clone();
    size_ = {zoomedHeight, zoomedWidth};
}

void Image::zoomIn(){
    int height = size_.first, width = size_.second;
    int zoomedHeight = (height*2) - 1, zoomedWidth = (width*2) - 1;
    Mat zoomed(zoomedHeight, zoomedWidth, CV_8UC3);

    for(int i=0; i<zoomedHeight; i+=2){
        for(int j=0; j<zoomedWidth; j+=2){
            zoomed.at<Vec3b>(i, j) = image_.at<Vec3b>(i/2, j/2);
        }
    }

    for(int i=0; i<zoomedHeight; i+=2){
        for(int j=1; j<zoomedWidth; j+=2){
            zoomed.at<Vec3b>(i, j)[0] = (int(zoomed.at<Vec3b>(i, j-1)[0]) + int(zoomed.at<Vec3b>(i, j+1)[0])) / 2;
            zoomed.at<Vec3b>(i, j)[1] = (int(zoomed.at<Vec3b>(i, j-1)[1]) + int(zoomed.at<Vec3b>(i, j+1)[1])) / 2;
            zoomed.at<Vec3b>(i, j)[2] = (int(zoomed.at<Vec3b>(i, j-1)[2]) + int(zoomed.at<Vec3b>(i, j+1)[2])) / 2;
        }
    }

    for(int i=1; i<zoomedHeight; i+=2){
        for(int j=0; j<zoomedWidth; j++){
            zoomed.at<Vec3b>(i, j)[0] = (int(zoomed.at<Vec3b>(i-1, j)[0]) + int(zoomed.at<Vec3b>(i+1, j)[0])) / 2;
            zoomed.at<Vec3b>(i, j)[1] = (int(zoomed.at<Vec3b>(i-1, j)[1]) + int(zoomed.at<Vec3b>(i+1, j)[1])) / 2;
            zoomed.at<Vec3b>(i, j)[2] = (int(zoomed.at<Vec3b>(i-1, j)[2]) + int(zoomed.at<Vec3b>(i+1, j)[2])) / 2;
        }
    }

    image_ = zoomed.clone();
    size_ = {zoomedHeight, zoomedWidth};
}