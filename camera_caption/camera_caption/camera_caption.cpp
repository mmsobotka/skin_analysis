#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <chrono>

int main() {
	cv::VideoCapture cap1(0, cv::CAP_ANY);
	cv::VideoCapture cap2(1, cv::CAP_ANY);

	if (!cap1.isOpened() || !cap2.isOpened()) {
		std::cerr << "Error: Couldn't open one or both cameras." << std::endl;
		return -1;
	}

	double fps = cap1.get(cv::CAP_PROP_FPS);
	int width = cap1.get(cv::CAP_PROP_FRAME_WIDTH);
	int height = cap1.get(cv::CAP_PROP_FRAME_HEIGHT);

	std::cout << "Camera 1: " << fps << " FPS, "
		<< width << "x" << height << std::endl;

	fps = cap2.get(cv::CAP_PROP_FPS);
	width = cap2.get(cv::CAP_PROP_FRAME_WIDTH);
	height = cap2.get(cv::CAP_PROP_FRAME_HEIGHT);

	std::cout << "Camera 2: " << fps << " FPS, "
		<< width << "x" << height << std::endl;

	const int bufferLimit = 100; // number of frames before save to file

	std::vector< cv::Mat> frames1;
	std::vector< cv::Mat> frames2;
	frames1.resize(bufferLimit);
	frames2.resize(bufferLimit);

	long long milliseconds[bufferLimit];

	for (int i = 0; i < bufferLimit; i++) {
		if (cap1.read(frames1[i]) && cap2.read(frames2[i])) {
			//skip frames
		}
	}


	auto start = std::chrono::steady_clock::now();
	unsigned int iFrame = 0;

	while (true) {
		if (cap1.read(frames1[iFrame]) && cap2.read(frames2[iFrame])) {

			auto end = std::chrono::steady_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			milliseconds[iFrame] = elapsedTime.count();

			// Display 
			cv::imshow("Camera 1", frames1[iFrame]);
			cv::imshow("Camera 2", frames2[iFrame]);

			iFrame++;

			// save frames to files if reaches the limit
			if (iFrame >= bufferLimit) {
				for (int i = 0; i < iFrame; i++) {
					char filename[100];
					snprintf(filename, sizeof(filename), ".//frames1//frame_%lld_cam_%d.jpg", milliseconds[i], 1);
					cv::imwrite(filename, frames1[i]);
				}

				for (int i = 0; i < iFrame; i++) {
					char filename[100];
					snprintf(filename, sizeof(filename), ".//frames2//frame_%lld_cam_%d.jpg", milliseconds[i], 2);
					cv::imwrite(filename, frames2[i]);
				}

				iFrame = 0;
			}

			// break loop if 'ESC' key pressed
			if (cv::waitKey(30) == 27) {
				break;
			}
		}

		else {
			std::cout << "couldnt read frame\n";
		}
	}

	if (cap1.isOpened()) {
		cap1.release();
	}

	if (cap2.isOpened()) {
		cap2.release();
	}

	return 0;
}