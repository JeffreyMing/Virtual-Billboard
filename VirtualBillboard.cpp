#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include "opencv2/opencv.hpp"


using namespace cv;
using namespace std;

Mat srcImg, dstImg;

//��m�y���I
vector <Point2f>pts_src;
vector <Point2f>pts_dst;

//�ƹ��ƥ��ơA�I��4���I
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
	//���o��l�v����4�Ө����y�Э�
	pts_src.push_back(Point2f(0, 0));
	pts_src.push_back(Point2f(srcImg.cols - 1, 0));
	pts_src.push_back(Point2f(srcImg.cols - 1, srcImg.rows - 1));
	pts_src.push_back(Point2f(0, srcImg.rows - 1));
	namedWindow("SrcWindow");
	imshow("SrcWindow", srcImg);

	dstImg = imread("../../data/images/times-square.jpg");
	Mat initial_dstImg = dstImg.clone();  //��l�ƥ�
	namedWindow("dstWindow");
	imshow("dstWindow", dstImg);

	//�Ұʷƹ�:�ؼмv��
	setMouseCallback("dstWindow", getHomographyMat_dst);

	while (1) {
		//�� Enter show�X Homography 
		if (waitKey(0) == 13) {
			Mat homoMat = findHomography(pts_src, pts_dst);   //�ѭ�l�Ϥ��M�I�諸4���I�A���o�ഫ�x�}
			Mat tempImg;
			warpPerspective(srcImg, tempImg, homoMat, dstImg.size());  //�z���ഫ
			Point pts_dst2[4];
			for (int i = 0; i < 4; i++) {
				pts_dst2[i] = pts_dst.at(i);
			}
			Mat dstImg2 = initial_dstImg.clone();
			fillConvexPoly(dstImg2, pts_dst2,4, Scalar(0,0,0), CV_AA);  //�N�I�諸4���I�������ϰ칳����R��0
			Mat outputImg;
			outputImg = tempImg + dstImg2;
			imshow("Output Image", outputImg);
		}
		//��l�ơAvector�ιϤ��A��c��C
		else if (waitKey(0) == 67 || waitKey(0) == 99) {
			pts_dst.clear();
			initial_dstImg.copyTo(dstImg);
			imshow("dstWindow", dstImg);
			cout << "clear points" << endl;
		}
	}

	waitKey(0);

}

