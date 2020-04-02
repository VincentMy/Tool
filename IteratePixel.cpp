#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void Iterate(Mat& src) {
	int height = src.rows;
	int width = src.cols;
	int ch = src.channels();
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			if (ch == 3) {									//三通道
				Vec3b pixels = src.at<Vec3b>(row,col);		//此处为3通道字节类型
				int blue = pixels[0];
				int green = pixels[1];
				int red = pixels[2];
				src.at<Vec3b>(row, col)[0] = 255 - blue;	//改变通道值
				src.at<Vec3b>(row, col)[1] = 255 - green;
				src.at<Vec3b>(row, col)[2] = 255 - red;
			}
			if (ch == 1) {									//单通道
				int pixel = src.at<uchar>(row, col);
				src.at<uchar>(row, col) = 255 - pixel;		//改变通道的值
			}
		}
	}
}
void Iterate_ptr(Mat& result, Mat& src) {					//指针方式获取像素信息
	int height = src.rows;
	int width = src.cols;
	int ch = src.channels();
	for (int row = 0; row < height; row++) {
		uchar* curr_row = src.ptr<uchar>(row);
		uchar* result_row = result.ptr<uchar>(row);
		for (int col = 0; col < width; col++) {
			if (ch == 3) {
				int blue = *curr_row++;
				int green = *curr_row++;
				int red = *curr_row++;

				*result_row++ = blue;
				*result_row++ = green;
				*result_row++ = red;
			}
			if (ch == 1) {
				int pixel = *curr_row++;
				*result_row++ = pixel;
			}
		}
	}
}
int main(int argc, char** argv) {
	Mat src = imread("D:/WorkSpaces/C++/arithmetic/test1.jpg",IMREAD_COLOR);
	if (src.empty()) {
		cout << "can not read image" << endl;
		return -1;
	}
	Mat zeros_src = Mat::zeros(src.size(), src.type());
	Iterate_ptr(zeros_src, src);						//指针类型的像素遍历
	Iterate(src);										//通过普通方式遍历像素
	
	imshow("pixel-demo", src);
	imshow("pixel-ptr-demo", zeros_src);
	waitKey(0);
	destroyAllWindows();
}