#include "ArucoHandler.h"

#include <opencv2/aruco.hpp>
#include <thread>

#include "../debuging/imgui/imgui.h"
#include "../debuging/DebugWindow.h"

#include "MarkerData.h"
namespace Aruco{
	void ArucoHandler::run() { 

		while (*isRunning) {
			if (camera.grab()) {
				cv::Mat img;

				// Get latest image
				camera.retrieve(img);

				if (img.empty())
					continue;

				lastImage = img;

				// Gray-scale image
				cv::Mat* grey = new cv::Mat(img.rows, img.cols, img.type());
				cv::cvtColor(img, *grey, cv::COLOR_BGR2GRAY);

				// Detect the markers
				Aruco::SimpleMarkerData simpleData = aruco.detectMarkers(*grey);

				delete grey;

				// Draw image and return if no code found
				if (simpleData.ids.size() <= 0)
				{
					DetectedMarkers.clear();
					continue;
				}

				// Calculate transform of markers
				Aruco::AdvancedMarkerData advancedData = aruco.estimateMarkerPosition(simpleData.corners);

				if (camvis::debugging::DebugWindow::isDebugEnabled())
					cv::aruco::drawDetectedMarkers(img, simpleData.corners);

				if (simpleData.ids.size() <= 0) continue;
				if (simpleData.ids.size() == 1) aruco.drawFrameAxes(img, simpleData.ids.size(), advancedData);

				std::vector<MarkerData> markerList;
				for (int i = 0; i < simpleData.ids.size(); i++) {

					MarkerData md = MarkerData(simpleData.ids[i], simpleData.corners[i], advancedData.tvecs[i], advancedData.rvecs[i], advancedData.rvecs[i], advancedData.tvecs[i]);
					markerList.push_back(md);

				}

				DetectedMarkers = markerList;
			}
		}
		camera.release();
	}

	void ArucoHandler::start() 
	{
		aruco = Aruco::ArucoVision("Resources/cam_params.yml", cv::aruco::DICT_6X6_250);

		// Starting the camera capture
		camera = cv::VideoCapture();
		camera.open(0);

		// Starting OpenCV Thread
		arucothread = (std::thread(&ArucoHandler::run, this));
		*isRunning = true;
	}

	void ArucoHandler::stop() {
		*isRunning = false;
		arucothread.join();
	}

	std::vector<MarkerData> ArucoHandler::getMarkers() 
	{
		return DetectedMarkers;
	}

	cv::Vec2d ArucoHandler::getCameraData() 
	{
		return cv::Vec2d(lastImage.rows, lastImage.cols);
	}

	cv::Mat ArucoHandler::getLastImage() {	

		cv::Mat convertedImage = cv::Mat(lastImage.rows, lastImage.cols, lastImage.type());

		if (lastImage.rows == 0) {
			std::cout << "image empty" << std::endl;
		}
		else {
			cv::cvtColor(lastImage, convertedImage, cv::COLOR_BGR2RGB);
		}
		return convertedImage;
	}

	void CalibrateCamera() 
	{

	}

}
