#include "ArucoHandler.h"

#include <opencv2/aruco.hpp>
#include <thread>

#include "MarkerData.h"
namespace Aruco {
	void ArucoHandler::run() {
		printf("Started marker detection");
		while (camera.grab() && isRunning) {
			cv::Mat img;
			camera.retrieve(img);
			cv::Mat grey;
			cv::cvtColor(img, grey, cv::COLOR_BGR2GRAY);
			Aruco::SimpleMarkerData simpleData = aruco.detectMarkers(grey);
			Aruco::AdvancedMarkerData advancedData = aruco.estimateMarkerPosition(simpleData.corners);

			

			if (simpleData.ids.size() <= 0)
			{
				cv::imshow("ArucoDebug", img);
				cv::waitKey(10);
				continue;
			}

			cv::aruco::drawDetectedMarkers(img, simpleData.corners, simpleData.ids);

			std::vector<MarkerData> markerList;
			for (int i = 0; i < simpleData.ids.size(); i++) {
				MarkerData md = MarkerData(simpleData.ids[i], simpleData.corners[i], advancedData.tvecs[i], advancedData.rvecs[i]);
				markerList.push_back(md);
			}
			DetectedMarkers = markerList;
			
			//aruco.drawFrameAxes(img, simpleData.ids.size(), advancedData);

			cv::imshow("ArucoDebug", img);
			cv::waitKey(10);
		}
		camera.release();
	}

	void ArucoHandler::start() {
		aruco = Aruco::ArucoVision("Resources/cam_params.yml", cv::aruco::DICT_6X6_250);
		camera = cv::VideoCapture();
		camera.open(0);

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
		return lastImage;
	}

	void CalibrateCamera() {

	}

}