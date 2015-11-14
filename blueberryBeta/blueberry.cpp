#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
using namespace cv;
using namespace std;

void CheckArguments(int argc) {
	if(argc != 2) {
		cout << "Error! Program usage:" << endl;
		cout << "./blueberry image_path" << endl;
		exit(-1);
	}	
}

void CheckImage(const Mat &img, const string &path) {
	if(img.empty()) {
		cout << "Unable to load image: " << path << endl;
		exit(-1);
	}	
}


int main(int argc, char **argv) {
	// Usage: ./blueberry images/blueberry.jpg
	CheckArguments(argc);

	// Load input image
	Mat bgr_image = imread(argv[1], 1);

	// Check if the image can be loaded
	CheckImage(bgr_image, argv[1]);

	Mat orig_image = bgr_image.clone();

	medianBlur(bgr_image, bgr_image, 3);

	// Convert input image to HSV
	Mat hsv_image;
	cvtColor(bgr_image, hsv_image, COLOR_BGR2HSV);

	// Threshold the HSV image, keep only the blue pixels
	Mat blueberry_range, blueberry_range2;
	inRange(hsv_image, Scalar(100, 100, 100), Scalar(150, 255, 255), blueberry_range);	//100 100 100		150	255	255

	equalizeHist(blueberry_range, blueberry_range2);
	//GaussianBlur(blueberry_range, blueberry_range, Size(9, 9), 2, 2);

	// Use the Hough transform to detect circles in the blue range
	vector<Vec3f> circles;
	HoughCircles(blueberry_range2, circles, CV_HOUGH_GRADIENT, 1, blueberry_range2.rows/8, 100, 20, 0, 0);

	// Loop over all detected circles and outline them on the original image
	if(circles.size() == 0)
		{
			printf("No blueberries detected \n");
			exit(-1);
	}

	// Check average size and discard "erroneous" circles
	int total_radius = 0;
	int discarded = 0;
	int detected = 0;

	for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle) 
	{
		Point center(round(circles[current_circle][0]), round(circles[current_circle][1]));
		total_radius = total_radius + round(circles[current_circle][2]);
		//circle(orig_image, center, radius, Scalar(0, 255, 0), 5);
	}

	int average_radius = total_radius / circles.size();

	//Draw detected circles
	for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle) 
	{
		Point center(round(circles[current_circle][0]), round(circles[current_circle][1]));
		int radius = round(circles[current_circle][2]);

		if(radius > 0.4 * average_radius && radius < 1.4 * average_radius)	//Check that it's a "normal" blueberry
		{
			printf ("%i \n", radius);
			circle(orig_image, center, radius, Scalar(0, 255, 0), 5);
		}
		else
			discarded++;
	}

	detected = circles.size()-discarded;

	printf ("Average radius: %i \n", average_radius);
	printf ("Blueberries discarded: %i \n", discarded);
	printf ("Blueberries detected: %i \n", detected);

	// Show images
	namedWindow("Getting only the blueberry range", WINDOW_AUTOSIZE);
	imshow("Getting only the blueberry range", blueberry_range2);
	namedWindow("Detected blueberries on the input image", WINDOW_AUTOSIZE);
	imshow("Detected blueberries on the input image", orig_image);

	waitKey(0);

	return 0;
}