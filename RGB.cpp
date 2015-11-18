#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
	Mat original_image;
	Mat hist;
	Mat edge_detected;
	Mat grey_image;
	Mat canny_edge;
	original_image = imread("Blueberry_white background.jpg", 1);
	if (original_image.empty())
	{
		cout << "Could not find image";
		return -1;
	}
	
	inRange(original_image, Scalar(0, 0, 0), Scalar(255, 140, 195), hist);
	

	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	dilate(hist	, hist, kernel, Point(-1, -1), 4);

	imshow("Ranged Image", hist);

	Canny(hist, edge_detected, 150, 200);
	imshow("Canny", edge_detected);

	//imshow("Dilated", edge_detected);

	vector<Vec3f> circles;
	HoughCircles(edge_detected, circles, CV_HOUGH_GRADIENT, 2, (original_image.rows / 6), 100, 50, 0, 0);
	int sum_radius = 0;
	for (size_t current_circle = 0; current_circle < circles.size(); current_circle++)
	{
		Point center(round(circles[current_circle][0]), round(circles[current_circle][1]));
		int radius = round(circles[current_circle][2]);
		sum_radius = sum_radius + radius;
	}
	int avg_radius = sum_radius / circles.size();
	for (size_t current_circle = 0; current_circle < circles.size(); current_circle++)
	{
		Point center(round(circles[current_circle][0]), round(circles[current_circle][1]));
		int radius = round(circles[current_circle][2]);
		sum_radius = sum_radius + radius;

		if ((radius<(1.5*avg_radius)) && (radius>0.5*avg_radius))
		{
			circle(original_image, center, radius, Scalar(0, 255, 0), 5);
		}
	}

	imshow("Hough circles detected", original_image);
	
	waitKey(0);
	return 0;

}


