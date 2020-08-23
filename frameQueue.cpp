#include <typeinfo>
#include <iostream>
#include <stdio.h> // here it is included for cerr
#include <thread> // Threading purpose
#include <queue> // For queue
//---------- OpenCV headers
#include <opencv2/videoio.hpp> // All video stream related
#include <opencv2/highgui.hpp> // For imshow and waitKey

// Hiccups
	// Camera input is not working tested on both python and cpp
	
int producer( std::queue<cv::Mat> &queue){
	cv::Mat frame; // Matrix
	cv::VideoCapture capture("../resources/people-walking.mp4"); // Getting feed

	if (!capture.isOpened()){
		std::cerr << "Error, Unable to open the camera\n";
		return -1;
	}
	while (true){
		capture.read(frame);
		queue.push(frame);
		if (frame.empty()){
			std::cerr << "Error, blank frame grabbed\n";
			break;
		}

		cv::imshow("Live", frame);

		if (cv::waitKey(20) >=0)
			break;
	}
}

int consumer(std::queue<cv::Mat> &queue){
}

int run(){
	std::queue<cv::Mat> queue; 
	std::thread prod(producer, std::ref(queue)); 
	prod.join();
	cv::Mat img = queue.front();
	cv::imshow("Pop", img);
	cv::waitKey(0);
	return 0;
}
