#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include "opencv2/opencv.hpp"


using namespace cv;
using namespace std;

Mat srcImg, dstImg;

//放置座標點
vector <Point2f>pts_src;
vector <Point2f>pts_dst;

//滑鼠事件函數，點選4個點
void getHomographyMat_dst(int event, int x, int y, int flags, void* userdata) {
	if (event == EVENT_LBUTTONDOWN) {
		if (pts_dst.size() < 4) {
			pts_dst.push_back(Point2f(x, y));
			circle(dstImg, Point(x, y), 3, Scalar(255, 255, 0), 1, CV_AA);
			cout << "dst image:" << Point2i(x, y) << endl;
			imshow("dstWindow", dstImg);
		}
	}
}

int main(void) {

	srcImg = imread("../../data/images/first-image.jpg");
	//取得原始影像的4個角的座標值
	pts_src.push_back(Point2f(0, 0));
	pts_src.push_back(Point2f(srcImg.cols - 1, 0));
	pts_src.push_back(Point2f(srcImg.cols - 1, srcImg.rows - 1));
	pts_src.push_back(Point2f(0, srcImg.rows - 1));
	namedWindow("SrcWindow");
	imshow("SrcWindow", srcImg);

	dstImg = imread("../../data/images/times-square.jpg");
	Mat initial_dstImg = dstImg.clone();  //初始化用
	namedWindow("dstWindow");
	imshow("dstWindow", dstImg);

	//啟動滑鼠:目標影像
	setMouseCallback("dstWindow", getHomographyMat_dst);

	while (1) {
		//按 Enter show出 Homography 
		if (waitKey(0) == 13) {
			Mat homoMat = findHomography(pts_src, pts_dst);   //由原始圖片和點選的4個點，取得轉換矩陣
			Mat tempImg;
			warpPerspective(srcImg, tempImg, homoMat, dstImg.size());  //透視轉換
			Point pts_dst2[4];
			for (int i = 0; i < 4; i++) {
				pts_dst2[i] = pts_dst.at(i);
			}
			Mat dstImg2 = initial_dstImg.clone();
			fillConvexPoly(dstImg2, pts_dst2,4, Scalar(0,0,0), CV_AA);  //將點選的4個點的內部區域像素填充為0
			Mat outputImg;
			outputImg = tempImg + dstImg2;
			imshow("Output Image", outputImg);
		}
		//初始化，vector及圖片，按c或C
		else if (waitKey(0) == 67 || waitKey(0) == 99) {
			pts_dst.clear();
			initial_dstImg.copyTo(dstImg);
			imshow("dstWindow", dstImg);
			cout << "clear points" << endl;
		}
	}

	waitKey(0);

}

