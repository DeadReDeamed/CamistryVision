#pragma once
#include "ArucoVision.h"

#include <opencv2/imgproc.hpp>

#include "MarkerData.h"
namespace Aruco {
	class ArucoHandler
	{
	private:
		Aruco::ArucoVision aruco;
		std::vector<MarkerData> DetectedMarkers;
		cv::Mat lastImage;
		std::thread arucothread;

		cv::VideoCapture camera;
		bool isRunning;
	private:
		void run();
	public:
		ArucoHandler() {}

		void start();
		void stop();
		std::vector<MarkerData> getMarkers();
		cv::Vec2d getCameraData();
		cv::Mat getLastImage();
	};

}