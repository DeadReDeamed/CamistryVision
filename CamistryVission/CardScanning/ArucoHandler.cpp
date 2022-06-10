#include "ArucoHandler.h"

#include <opencv2/aruco.hpp>
#include <thread>

#include "MarkerData.h"
namespace Aruco {

	void ArucoHandler::run() {

		while (camera.grab() && isRunning) {
			cv::Mat img;

			// Get latest image
			camera.retrieve(img);
			if (img.empty())
				return;

			lastImage = img;

			// Gray-scale image
			cv::Mat grey = cv::Mat();
			cv::cvtColor(img, grey, cv::COLOR_BGR2GRAY);
			
			// Detect the markers
			Aruco::SimpleMarkerData simpleData = aruco.detectMarkers(grey);

			// Draw image and return if no code found
			if (simpleData.ids.size() <= 0)
			{
				cv::imshow("ArucoDebug", img);
				cv::waitKey(1);
				DetectedMarkers.clear();
				continue;
			}

			//cv::aruco::drawDetectedMarkers(img, simpleData.corners, simpleData.ids);

			// Calculate transform of markers
			Aruco::AdvancedMarkerData advancedData = aruco.estimateMarkerPosition(simpleData.corners);

			std::vector<MarkerData> markerList;
			for (int i = 0; i < simpleData.ids.size(); i++) {

				MarkerData md = MarkerData(simpleData.ids[i], simpleData.corners[i], advancedData.tvecs[i], advancedData.rvecs[i], advancedData.rvecs[i], advancedData.tvecs[i]);
				markerList.push_back(md);

			}

			DetectedMarkers = markerList;

			cv::imshow("ArucoDebug", img);
			cv::waitKey(1);
		}
		camera.release();
	}

	void ArucoHandler::start() {
		aruco = Aruco::ArucoVision("Resources/cam_params.yml", cv::aruco::DICT_6X6_250);

		// Starting the camera capture
		camera = cv::VideoCapture();
		camera.open(0);

		// Starting OpenCV Thread
		arucothread = (std::thread(&ArucoHandler::run, this));
		isRunning = true;
	}

	void ArucoHandler::stop() {
		isRunning = false;
		arucothread.join();
	}

	std::vector<MarkerData> ArucoHandler::getMarkers() {
		return DetectedMarkers;
	}

	cv::Vec2d ArucoHandler::getCameraData() {
		return cv::Vec2d(lastImage.rows, lastImage.cols);
	}

	cv::Mat ArucoHandler::getLastImage() {	

		if (lastImage.empty())
			return lastImage;

		//return lastImage;
		cv::Mat convertedImage = cv::Mat();
		cv::cvtColor(lastImage, convertedImage, cv::COLOR_BGR2RGB);
		return convertedImage;
	}

	void CalibrateCamera() {

	}

}