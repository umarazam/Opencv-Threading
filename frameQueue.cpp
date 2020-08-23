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
	
std::mutex m;
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
		/**
		if (frame.empty()){
			std::cerr << "Error, blank frame grabbed\n";
			break;
		}
		**/

		cv::imshow("Live", frame);

		if (cv::waitKey(20) >=0)
			break;
	}
	return 0;
}

void consumer(std::queue<cv::Mat> &queue){
	int choice = -99;
	while (true){
	std::cout << "Want to look at the frame then press 1: ";
	std::cin >> choice;
	if (choice == 1){
		choice = -99;
		m.lock();
		cv::Mat img = queue.front();
		m.unlock();
		cv::imshow("Pop", img);
		//cv::waitKey(0);
		}
	}
}

int run(){
	std::queue<cv::Mat> queue; 
	while (true){
	std::thread prod(producer, std::ref(queue)); 
	std::thread cons(consumer, std::ref(queue));

	prod.join();
	cons.join();
	}
		
	return 0;
}
